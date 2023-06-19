#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"


/*
--------------------
	Listener 정의부
--------------------
*/
Listener::~Listener()
{
	// 소멸 시점에 Listener가 들고있던 소켓을 닫아줍니다. 
	SocketUtils::Close(_socket);

	// 반복문을돌면서 들고있던 AcceptEvent 들도 정리합니다.
	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO 

		A_delete(acceptEvent);
	}

}

// Listener를 만든 다음 그 소켓으로 Accept를 호출합니다.
bool Listener::StartAccept(ServerServiceRef service)
{
	// _service를 채워줍니다. 
	_service = service;
	if (_service == nullptr)
		return false;

	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;
 
	/*
	지금까지는 임시로 IocpCore 객체를 그냥 extern 으로 만들고 있었는데 이부분을 수정해주겠습니다. 
	IocpCore 클래스에서 삭제를 했지만 이제는 _service에서 꺼내 사용할 수 있습니다. 
	그리고 IocpCore->Register() 의 인자로 그냥 this 를 넘겨주면 안되고 
	shared_from_this() 로 넘겨줘야합니다 
	*/
	if (_service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	if (SocketUtils::Bind(_socket, _service->GetNetAddress()) == false)
		return false;

	if(SocketUtils::Listen(_socket) == false)
		return false;

	// 이제 acceptCount도 ServerService의 maxSessionCount 값으로 주겠습니다. 
	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = A_new<AcceptEvent>();
		acceptEvent->owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

// 이 함수는 별건아니고 그냥 이 lisnteSocket을 닫아줍니다.
void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

// _socket을 HANDLE로 캐스팅해 반환
HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

// IocpCore::Dispatch 에서 GetQueuedCompletionStatus 를 통과한후 IocpEvent->Dispatch를 호출한게 이곳으로 옵니다.
void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// 현재까지 테스트중인 Event 타입이 Accept 뿐이니까 에러체크해줍니다.
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);

	// iocpEvent를 AcceptEvent로 복원해주겠습니다. 
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	
	// 이제 이 AcceptEvent를 실행하기위해 ProcessAccept 를 호출합니다
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	// 이전 MakeShared 로 생성해주던 session 객체는 _service->CreateSession() 으로 대체합니다.
	// CreateSession() 에서는 객체 생성뿐만아니라 그 세션을 CP에 등록까지 해줍니다. 
	SessionRef session = _service->CreateSession();
	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		// 실패했으면 에러코드를 봅니다.
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// Pending 상태가 아니라면 문제가 있는 상황입니다.
			// 이경우에는 RegisterAccept 호출하던게 끊긴 상황이니 직접 다시 걸어줘야 합니다.
			RegisterAccept(acceptEvent);
		}
	}
}


void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	// AcceptEvent에 보관해뒀던 세션을 꺼냅니다. 
	SessionRef session = acceptEvent->session;

	// SetUpdateAcceptSocket 이라는 함수는 인자 소켓과 listen 소켓의 옵션을 똑같이 맞춰줍니다.
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		// 실패했으면 리턴하기전에 꼭 RegisterAccept를 호출해줘서 다음 예약을 해줍니다. 
		RegisterAccept(acceptEvent);
		return;
	}

	// 데이터를 추출해보겠습니다. getpeername 함수를 통해 sockAddress에 주소를 가져오고있습니다. 
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	// 가져온 주소를 NetAddress 객체로 만들어 세팅합니다. 
	session->SetNetAddress(NetAddress(sockAddress));

	// 여기까지 왔으면 성공적으로 연결되었다고 볼 수 있습니다.
	cout << "Client Connected!" << endl;

	// TODO

	RegisterAccept(acceptEvent);

	// 위 내용들을 보면 한번 클라이언트가 접속하고 생성한 세션과 IocpEvent는 계속 재사용하고 있습니다. 
}
