#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"


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
bool Listener::StartAccept(NetAddress netAddress)
{
	// 일단 Listener의 소켓을 만듭니다. 
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	// 다음으로 Listener를 CP에 등록합니다. 
	if (GIocpCore.Register(this) == false)
		return false;

	// 소켓 옵션중 주소 재사용 가능하게 해서 주소가 곂쳐도 동작가능하게 해줍니다. 
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	// Linger라는 옵션은 만약 데이터 전송도중 접속이 끊겼을때 보내고 있던 데이터를 잠시라도 
	// 보존하겠는가? 보존한다면 어느정도 시간동안 보존할것인지에 대한 옵션입니다. 
	// 여기서는 둘다 0, 0 으로 넣어 접속이 끊기면 그냥 데이터를 다 날리는걸로 하였습니다. 
	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	// 어느정도 소켓옵션을 다 정했으면 Bind로 listenSocket과 서버 자신의 주소를 묶습니다. 
	if (SocketUtils::Bind(_socket, netAddress) == false)
		return false;

	if(SocketUtils::Listen(_socket) == false)
		return false;


	//여기까지 소켓옵션, Bind, Listen 까지 했으면 AcceptEx를 호출예약할 차례입니다.
	//RegisterAccept 를 사용해서 AcceptEx 를 예약합니다. 이후 누군가가 접속하면 완료 통지가 뜨게 될겁니다. 
	//완료 통지는 CP를 관찰하던 Worker 스레드에서 포착할겁니다.
	//그리고 한번에 접속요청이 몰릴때 Accept 예약이 하나뿐이면 몇몇개는 놓칠수도 있기때문에 for문으로 
	//복수의 예약을 걸어놓으면 좋습니다. 일단 지금은 하나만 예약합니다.
	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = A_new<AcceptEvent>();
		// 이 반복문 안에서 만든 acceptEvent를 _acceptEvents 라는 벡터에서도 참조하게 해서 
		// 나중에 삭제하기 쉽도록 합니다. 소멸자 호출때 알아서 삭제해줍니다.
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return false;
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
	ASSERT_CRASH(iocpEvent->GetType() == EventType::Accept);

	// iocpEvent를 AcceptEvent로 복원해주겠습니다. 
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	
	// 이제 이 AcceptEvent를 실행하기위해 ProcessAccept 를 호출합니다
	ProcessAccept(acceptEvent);
}

// AcceptEx를 예약해주는 함수 즉, 실질적으로 AcceptEx를 호출해줍니다.
// 여기서 예약해둔 AcceptEx는 누군가 접속해주게되면 Dispatch로 넘어갈겁니다.
void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	/*
	AcceptEx가 받는 인자가 많은데 클라이언트에 대한 인자가 많습니다.
	각 인자를 지금 이 함수 내에서 만들기보다는 Session이라는 클라이언트에 대한 데이터들을 모아둔 
	객체에서 가져와 사용하는게 좋습니다. 
	나중에는 미리 준비된 세션을 풀링해와서 사용해도 되지만, 지금은 접속 테스트까지만 해볼것이니
	간소하게 여기서 생성했습니다.
	이번시간에 추가로 AcceptEvent 클래스에서 세션을 기억할수 있도록 만들었습니다. 
	왜냐면 Dispatch나 ProsessAccept 에서 AcceptEvent를 받아 실행이 될텐데 
	어떤 세션과 연동되어있는지를 확인할수 있도록 합니다.
	*/
	Session* session = A_new<Session>();
	acceptEvent->Init();
	acceptEvent->SetSession(session);

	/*
	AcceptEx의 인자목록
	1) listener Socket
	2) AcceptEx 호출후 clientSocket이 될 소켓
	3) lpOutputBuffer 처음 연결되면서 서버의 로컬주소 및 클라이언트의 원격주소를 수신할 버퍼
	4) dwReceiveDataLength 위의 서버 로컬주소, 클라의 원격주소를 제외한 연결과 동시에 수신할 데이터의 크기
		0을 넣으면 연결과 동시에 수신하지 않고 바로 AcceptEx를 완료합니다.
	5) dwLocalAddressLength 로컬주소를 위해 예약된 바이트 수입니다. 현재 사용중인 전송프로토콜
		SOCKADDR_IN 보다 16바이트 이상이어야 합니다. 
	6) dwRemoteAddressLength 원격주소를 위해 예약된 바이트 수입니다. 와야 할 값은 위와 같습니다.
	7) lpdwBytesReceived 연결하면서 받은 데이터의 바이트 수를 수신할 인자입니다. DWORD* 타입입니다.
	8) lpOverlapped 요청을 처리할때 사용되는 LPOVERLAPPEC 구조체입니다. 우리는 IocpEvent를 캐스팅해 넣어주면됩니다. 
	*/
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
	Session* session = acceptEvent->GetSession();

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
