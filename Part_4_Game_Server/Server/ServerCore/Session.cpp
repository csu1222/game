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

void Session::Disconnect(const WCHAR* cause)
{
	// exchage의 인자로 _connected의 값이 바뀌고 반환은 이전 가지고 있던 값을 반환합니다.
	// 그래서 아래 조건문이 참이라는것은 이미 _connected의 값이 false 였다는 말입니다. 
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnet : " << cause << endl;

	OnDisconnected();	// 컨텐츠 코드에서 오버로딩

	SocketUtils::Close(_socket);	// 소켓도 닫습니다.
	GetService()->ReleaseSession(GetSessionRef());	// Release Ref

	// 이제 Session의 참조 카운트가 최종적으로 0이 되면서 소멸합니다.
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// 여기까지 들어왔다는건 IocpCore::Dispatch가 다 완료되고 등록했던 비동기 IO 함수가 완료되었다는 이야기

	// IocpEvent의 타입에 따라 하는일이 갈립니다. 이번 시간에는 일단 Recv만 구현합니다.
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
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


	/*
	WSARecv의 인자중 OVERLAPPED 구조체에 해당하는 IocpEvent 객체, Session에서는 RecvEvent를 
	현재 함수에서 매번 새로 만들어 주는 방법도 있고 아니면
	IocpEvent들은 각 세션마다 할당되어 동작할것이기 때문에 멤버변수로 들고 있고 그걸 넘겨주는 방법도 있습니다.
	지금은 내부에 멤버 변수로 Event를 들고 있는 방식으로 해보겠습니다. 
	*/
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		// WSARecv 가 진짜 실패했는지를 체크
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// 펜딩 상태가 아니면 정말 무언가 문제가 생긴것
			// HandleError 함수로 어떤 에러인지를 출력하고 현제 세션의 참조카운트를 줄입니다.
			HandleError(errorCode); 
			_recvEvent.owner = nullptr;
		}
		// 에러코드가 펜딩상태이면 그냥 아직 들어온 데이터가 없다는 뜻
		// 데이터가 들어올때 까지 관찰(Dispatch) 
	}
}

void Session::RegisterSend()
{
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

	// TODO 
	cout << "Recv Data Len = " << numOfBytes << endl;

	// 다음번 수신을 위해 다시 Register를 겁니다.
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
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
