#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"
#include "IocpEvent.h"

/*
--------------------
	Session 정의부 
--------------------
*/
Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Send(BYTE* buffer, int32 len)
{
	/*
	생각할 문제 
	1) 버퍼 관리?
	2) sendEvent 관리? 단일? 여러개? WSASend 중첩?
	- MMORPG를 가정했을때 Send를 한번에 한번씩 차례대로 호출하는게 아니라 몬스터 사냥, 귓속말 같은 
	  한번 한번 일어날때 마다 중첩해서 Send를 호출해줘야 합니다. 그럴경우 _recvEvent 처럼 
	  딱 하나만 만들어서 사용하다보면 중첩된 Send 호출을 감당할 수 없습니다. 
	  지금은 임시로 Send함수 안에서 실시간 생성하는방식으로 구현해보겠습니다. 
	*/

	// TEMP
	SendEvent* sendEvent = A_new<SendEvent>();
	sendEvent->owner = shared_from_this();		// ADD_REF
	// 임시로 SendEvent가 버퍼를 들고 있을것이고 거기에 보낼데이터를 저장해줍니다. 
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	/*
	항상 Register, Process 순으로 진행했었습니다.보낼 버퍼와 연결된 sendEvent 를 인자로 받습니다.
	Send의 경우 채팅, 아이템 획득, 경험치 획득 같이 주기적으로 호출되는게 아니라 특정상황에 한번에 여러
	스레드로 호출될 수 있습니다. 
	그럴때 RegisterSend 안의 WSASend 함수가 스레드 환경에서 안전한지를 알아야 하는데 공식문서를 보면 
	스레드에서 안전이 보장되어 있지 않다고 합니다. 그러면 우리가 직접 순서를 보장해줘야 합니다. 
	*/
	WRITE_LOCK;
	RegisterSend(sendEvent);

}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnet : " << cause << endl;

	OnDisconnected();	// 컨텐츠 코드에서 오버로딩

	SocketUtils::Close(_socket);	// 소켓도 닫습니다.
	GetService()->ReleaseSession(GetSessionRef());	// Release Ref

}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:	// 지금은 SendEvent를 인자로 넘겨줘야합니다.
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
		break;
	}
}

void Session::RegisterConnect()
{
}

void Session::RegisterRecv()
{
	// 접속여부 부터 체크
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	// _recvBuffer는 아직 임시로 사용하는 중이고 나중에 수정할 예정
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);	
	wsaBuf.len = len32(_recvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	// owner를 설정하는것으로 사실상 owner의 Ref Count를 1늘린셈
	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();


	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		// WSARecv 가 진짜 실패했는지를 체크
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode); 
			_recvEvent.owner = nullptr;
		}	}
}

void Session::RegisterSend(SendEvent* sendEvent)
{
	/*
	Send 함수에서 락을 걸어주었습니다. 이제 순서대로 WSASend가 호출이 될 텐데 아무리 많은 
	스레드에서 WSASend를 시도하더라도 락을 통해 먼저 시도한 순서대로 예약을 하게 됩니다. 
	그 다음 IocpCore::Dispatch 안에 GetQueuedCompletionStatus 를 호출해줄텐데 
	이부분에서는 순서보장을 할 수 없습니다. IocpCore 에서는 따로 락을 사용하지 않기 때문입니다. 
	그래서 데이터를 보내는 순서가 중요한 경우는 문제가 될 수 있습니다.

	또 한가지 WSASend를 호출한 다음 Pending 상황이 되었다는것은 커널쪽 SendBuffer가 꽉차 있다는 이야기입니다.
	그 상황에서 무작정 다음 WSASend를 계속 밀어넣는게 좋은지는 생각해 봐야 합니다. 
	우리가 WSASend를 호출할때 WSABUF의 내용물로 buf와 len을 넘겨주는데 Pending 상황에서는 
	지금당장 커널쪽으로 이 WSABUF 를 넘겨줄 수 없다고 해도 언젠가는 커널쪽 버퍼로 복사가 이뤄질겁니다.
	고급 내용이라서 자세히는 알아보지 않을테지만 이럴때 WSABUF 를 대상으로 페이지락 이라는것을 건다고 합니다.

	그리고 WSASend를 호출할때 작은데이터를 각각 여러번 호출하는것보다 데이터를 한번에 모아서 
	WSASend를 호출하는게 좋습니다. 인자에서도 WSABUF 와 이 WSABUF가 몇개인지를 받습니다.
	즉, 모아서 보낼수 있도록 되어 있습니다. 

	현재 작업중인 코드는 이러한 부분들이 아쉽습니다. 순서보장이 완전히 되는것도 아니고 
	자잘한 데이터라도 매번 RegisterSend를 호출하게 되는데
	그러지 말고 보낼 데이터를 멤버 변수에 저장하다가 일정 크기가 되면 한번에 보낼 수 있도록 해주면 
	좀 더 성능상으로 유리하다고 할 수 있습니다. 
	*/

	// 접속여부 부터 체크
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)sendEvent->buffer.data();
	wsaBuf.len = (ULONG)sendEvent->buffer.size();

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, OUT & numOfBytes, 0, sendEvent, nullptr))
	{
		// 진짜 실패인지 PENDING 상태인지 체크 
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent->owner = nullptr;
			//실시간으로 생성했던 sendEvent는 일이 끝났으니 삭제
			A_delete(sendEvent);
		}
	}
	// 이후 IocpCore->Dispatch 를 통해 ProcessSend로 넘어갈것입니다.
	// 그런데 여기서도 마찬가지로 Session가 멤버 변수로서 SendEvent를 들고 있는게 아니기 때문에 
	// 다시 인자로 넘겨주도록 해줘야 합니다. 
	
}

void Session::ProcessConnect()
{
	_connected.store(true);

	// 세션 등록 : 이코드에 와서야 서비스에 세션을 실제로 연결해주는겁니다. 
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 오버로딩할 예정
	OnConnected();
		
	// 수신 등록 : 연결 후 여기서 부터 Recv를 Iocp에 등록을 해줍니다. 
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	// 더이상 WSARecv 가 예약이 걸린 상태가 아니니 이 세션의 참조카운트를 줄여줍니다. 
	_recvEvent.owner = nullptr;

	if (numOfBytes == 0)
	{
		// Dispatch를 통과한 상태에서 받은 데이터가 0이라면 연결이 끊겼다는 이야기
		Disconnect(L"Recv 0");
		return;
	}

	// TODO : 이전에 누락한 부분인데 로그찍는 코드는 생략하고 OnRecv라는 컨텐츠 쪽에서 오버로딩해 사용할 함수를 호출합니다. 
	// cout << "Recv Data Len = " << numOfBytes << endl;
	OnRecv(_recvBuffer, numOfBytes);

	// 다음번 수신을 위해 다시 Register를 겁니다.
	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	// ProcessRecv와 마찬가지로 성공했다면 이 세션의 참조카운트를 줄여줍니다.
	sendEvent->owner = nullptr;
	A_delete(sendEvent);

	if (numOfBytes == 0)
	{
		// Dispatch를 통과한 상태에서 보낸 데이터가 0이라면 연결이 끊겼다는 이야기
		Disconnect(L"Recv 0");
		return;
	}

	// 컨텐츠 코드에서 오버로딩
	OnSend(numOfBytes);

}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}
