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
Session::Session() : _recvBuffer(BUFFER_SIZE)
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Send(SendBufferRef sendBuffer)
{
	// 연결이 되어있는지 체크
	if (IsConnected() == false)
		return;

	// 스택 메모리에 이 Send명령을 예약해야하는지를 만들었습니다.
	bool registerSend = false;

	// 락을 거는 범위를 조절합니다. 
	{
		WRITE_LOCK;

		_sendQueue.push(sendBuffer);

		if (_sendRegistered.exchange(true) == false)
			registerSend = true;
	}

	// 락의 범위 밖에서 RegisterSend를 호출하면서 스레드 끼리 정체되는걸 줄였습니다.
	if (registerSend)
		RegisterSend();
}

bool Session::Connect()
{
	/*
	Connect 말 그대로 연결을 하는 함수 입니다. 서버 입장에서는 Accept를 기다리고 있다가 
	접속한 클라를 상대로 통신을 하면 되기 때문에 사실 Connect가 왜 필요한지 감이 안 올 수 있는데
	이전에 한번 언급했던 적이 있습니다. 서버도 단일서버로 동작할때도 있고 다중서버로 역할을 나눠서 
	동작하는 경우도 있는데 다중 서버일때 서버끼리 연결을 할때 이 함수가 필요해집니다.

	*/
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	/*
	소켓을 닫을때 그냥 Session이 들고 있는 소켓을 강제로 닫아주고 있습니다. 
	꼭 이게 문제라기 보다는 이렇게 소켓을 닫아줘도 연결이 끊기기는 합니다.
	실제로도 이런방식으로 연결을 끊는 경우도 있기는한데 
	우리가 SocketUtils 에서 런타임에 받아줬던 함수들중 DisconnectEx 라는것도 있습니다.
	비동기 IO 함수중 하나인데 이걸 사용하면 호출하면 IOCP에 예약을 해놓고 
	처리할 준비가 되면 쓰레드에서 처리해주는 방식입니다. 
	
	그러면 DisconnectEx를 사용하는 장점이 무엇인가하면 
	지금은 Session 생성자, 소멸자, 그리고 여기 Disconnect에서 소켓을 만들고 닫아주고 하는데
	사실 이 작업이 부담이 많이 가는 작업이라고 합니다. 그런만큼 한번 만든 소켓을 재사용한다면 
	성능상 더욱 이득이 될겁니다. 
	성능상으로도 좋고 어짜피 Iocp를 이용하고 있는데 굳이 DisconnectEx를 사용하지 않을 이유도 
	없으니깐 이 방법으로 배워보겠습니다. 
	*/
	
	// _connected의 값을 false 로 넣어주는데 이미 값이 false였으면 반환도 false가 반환됩니다.
	// 즉, 이 Disconnect 함수를 한번만 호출하도록 한것입니다. 
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnet : " << cause << endl;

	
	// SocketUtils::Close 대신 RegisterDisconnect 를 호출합니다. 
	RegisterDisconnect();
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
	case EventType::Disconnect:
		ProcessDisconnect();
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:	// SendEvent를 멤버 변수로 들고 있게 되어 인자로 받지 않습니다.
		ProcessSend(numOfBytes);
		break;
	}
}

bool Session::RegisterConnect()
{
	// 이미 연결된 상태인지 체크 
	if (IsConnected())
		return false;

	// 현재 이 세션이 속한 서비스의 타입이 클라이언트인지를 체크합니다. 
	// 클라이언트타입이 다른 서버에 접속하고 서버 타입은 상대방이 나에게 연결을 시도하는것입니다. 
	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	// 소켓의 옵션들을 조정해줍니다. 
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;
	
	// Listener쪽 소켓 옵션을 설정할때는 하지 않았는데 클라입장에서는 아무 Address, Port 를 
	// 사용하도록 했습니다. 
	if (SocketUtils::BindAnyAddress(_socket, 0/*0을 주면 남는 포트 아무거나 연결*/) == false)
		return false;
	
	// Connect는 동시에 여러번 일어나는 작업이 아니기 때문에 직접 멤버 변수로 들고 있어도 됩니다.
	_connectEvent.Init();
	_connectEvent.owner = shared_from_this();	// ADD_REF

	// ConnectEx 에 필요한 인자들
	DWORD numOfBytes = 0;	// 접속과 동시에 보낸 데이터의 크기를 받을것입니다.
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr(); // 내가 어디로 연결할지

	if (SOCKET_ERROR == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		// Pending상태인지 까지 체크
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// Pending 상태가 아니라면 진짜로 실패
			_connectEvent.owner = nullptr;	// RELEASE_REF
			return false;
		}
		// Pending 상태라면 좀 더 대기
	}
	return true;
}

bool Session::RegisterDisconnect()
{
	// 반복되는 구조입니다.
	// _disconnectEvent 초기화, 자신을 등록해 ADD_REF
	_disconnectEvent.Init();
	_disconnectEvent.owner = shared_from_this();	// ADD_REF

	/*
	DisConnectEx 함수의 인자입니다. 
	1) 소켓
	2) OVERLAPPED 의 포인터 우리는 IocpEvent 객체를 주면됩니다.
	3) 함수 호출에 대한 플래그 입니다. 0을 주면 아무런 플래그가 설정되지 않고 
	   지금 처럼 TF_REUSE_SOCKET 을 주면 DisconnectEx 호출 후 다시 AcceptEx나 ConnectEx에서
	   이 소켓을 사용할 수 있습니다.
	4) reserved 를 줘야한다고 하는데 꼭 0을 줘야합니다. 다른 값을 주면 잘못된 인자를 줬다는 에러가 뜹니다.
	*/
	if (false == ::SocketUtils::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_disconnectEvent.owner = nullptr;	// RELEASE_REF
			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	// 접속여부 부터 체크
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	// wsaBuf.buf 는 _recvBuffer의 시작부터 쓰지않고 _writePos 부터 쓰기 시작
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	// len은 받을 버퍼의 사이즈가 아닌 버퍼의 여유공간을 뜻합니다. 
	wsaBuf.len = _recvBuffer.FreeSize();

	// 커널에서 실제로 수신한 버퍼의 사이즈는 numOfBytes로 받아줄것입니다. 
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


void Session::RegisterSend()
{
	// RegisterSend를 Send에서 매번 호출하는게 아닌 처음만 호출하고 ProcessSend에서 다음번 
	// RegisterSend를 호출하도록 하면서 _sendEvent를 멤버 변수로 들고 있을 수 있게 되었습니다. 
	// Send에서 WRITE_LOCK 을 사용하면서 한번에 한 스레드에서만 호출되도록 되었습니다.
	if (IsConnected() == false)
		return;
	
	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();	// ADD_REF

	// 보낼 데이터를 SendEvent에 등록 
	{
		// 왜 중첩해서 락을 잡느냐 하면 나중에 혹시 RegisterSend를 락을 안잡고 호출하게 
		// 바뀔 수 도 있기 때문에 2중으로 잡고 있습니다. 
		WRITE_LOCK;
		
		// 기존 SendEvent.buffer는 BYTE 타입을 받는 vector였습니다. 지금은 SendBufferRef를 받는 
		// vector로 수정했습니다. 

		// Send때 _sendQueue에 넣어두었던 SendBufferRef를 꺼내 sendEvent에 연결해주고 있는데 왜 굳이 이렇게 하느냐?
		// 보낼 SendBufferRef가 WSASend를 호출한 후부터는 사라지지 않도록 보존을 해야하는데 
		// 중간에 _sendQueue 에서 꺼내다가 SendBufferRef의 RefCount 가 0이 되면 삭제가 될 수 있기때문에
		// 삭제 되지 않도록 SendEvent가 참조 하고 있게 하는것입니다. 

		// _sendQueue가 빌때 까지 반복하는 것으로 간단하게 만들겠습니다. 
		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();

			// 꺼낸 버퍼들의 writeSize를 더해가면서 추적하는 이유는 
			// 실제 버퍼의 capacity를 넘으면 안되기 때문에 중간에 끊어 주기 위함
			writeSize += sendBuffer->WriteSize();

			// TODO : 예외 체크 
			
			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}



	// WSABUF 를 보낼때 여러 개를 뭉쳐서 한번에 보낼 수도 있었습니다. 
	// Scatter-Gather (흩어져 있는 데이터들을 모아서 한방에 보낸다)

	Vector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.sendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;

		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WriteSize());

		wsaBufs.push_back(wsaBuf);
	}


	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket,/*동적배열의 시작주소*/ wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &_sendEvent, nullptr))
	{
		// 진짜 실패인지 PENDING 상태인지 체크 
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_sendEvent.owner = nullptr; // RELEASE_REF
			_sendEvent.sendBuffers.clear(); // RELEASE_REF
			_sendRegistered.store(false);
		}
	}
}

void Session::ProcessConnect()
{
	/*
	이 ProcessConnect 함수는 두가지 용도로 사용하게 됩니다. 
	Listener::ProcessAccept 함수에서 호출되는 이 서버에 클라가 접속할때 도 사용하고 
	이 서버가 클라이언트 서버로서 다른 서버에 접속하려고 할때도 사용됩니다. 
	*/

	_connectEvent.owner = nullptr;	// RELEASE_REF

	_connected.store(true);

	// 세션 등록 : 이코드에 와서야 서비스에 세션을 실제로 연결해주는겁니다. 
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 오버라이딩할 예정
	OnConnected();
		
	// 수신 등록 : 연결 후 여기서 부터 Recv를 Iocp에 등록을 해줍니다. 
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	// 여기서는 접속 종료후에 할일인데 사실 연결이 끊긴뒤에는 딱히 할 일이없고 
	// 세션의 참조카운트만 줄이고 마치겠습니다. 
	_disconnectEvent.owner = nullptr;	// RELEASE_REF

	OnDisconnected();

	GetService()->ReleaseSession(GetSessionRef());
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr;

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	// 여기 까지 들어왔다는것은 IocpCore::Dispatch 에서 처리가 되어 
	// _recvBuffer에 데이터가 복사 되었다는 말입니다.
	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		// OnWrite가 실패할 일은 많이 없지만 예외 처리는 꼼꼼히 해줍니다. 
		Disconnect(L"OnWrite Overflow");
		return;
	}

	// 다음은 지금까지 write 한 만큼의 데이터 사이즈르 받습니다. 
	int32 dataSize = _recvBuffer.DataSize();

	// _readPos 부터 쌓인 데이터 만큼을 인자로 컨텐츠 쪽으로 넘겨줘서 처리를 해주게 될겁니다. 
	// 테스트 코드에서는 데이터를 처리한 셈 치고 그 사이즈를 콘솔에 출력해주고 있습니다. 
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);

	// OnRecv에서 쌓인 데이터를 처리했다는것은 Read를 했다는 게 됩니다. 
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
	{
		// processLen 값이 음수이거나 dataSize 보다 큰것은 말이 안되는 상황이고,
		// 실질적으로 _recvBuffer.OnRead(processLen) 으로 _readPos를 땡긴 작업이 실패 했다면 문제가 있는 상황입니다. 
		Disconnect(L"OnRead Overflow");
		return;
	}

	// Read, Write를 다 했으면 커서 정리를 해줍니다.
	_recvBuffer.Clean();

	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
	_sendEvent.owner = nullptr;	// RELEASE_REF
	_sendEvent.sendBuffers.clear(); // RELEASE_REF
	

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	// 컨텐츠 코드에서 오버라이딩
	OnSend(numOfBytes);

	// _sendQueue에 데이터가 남았는지 아닌지에 따라 계속 RegisterSend를 호출해줄것인지 
	// 다음번 Send호출을 기다릴것인지 나뉩니다.
	WRITE_LOCK;	// _sendQueue는 멀티스레드 환경에서 동시 접근할 수 있기 때문
	if (_sendQueue.empty())
		_sendRegistered.store(false);
	else
		RegisterSend();
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



/*
--------------------------
	PacketSession 정의부
--------------------------
*/

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	// [size(2)][id(2)][data......][size(2)][id(2)][data......]...
	// 이런 형태로 패킷이 오게 만든다고 했습니다. 
	// 일단 무한 루프 안에서 진행합니다.

	int32 processLen = 0;	// 데이터 처리가 얼만큼 진행됐는지를 표현할것입니다. 

	while (true)
	{
		// OnRecv로 받은 총 버퍼의 크기에 처리한 패킷의 크기를 빼줘서 
		// 반복할때마다 패킷하나만큼의 크기씩 줄어들게 합니다. 
		int32 dataSize = len - processLen;

		// 최소한 len이 패킷헤더가 들어 있어야 합니다. 
		if (dataSize < sizeof(PacketHeader))
			break;

		// 최소 패킷헤더는 포함되었다면 헤더 내용을 확인 해봅니다. 
		// 사실 id는 지금은 필요없고 size가 필요합니다. 
		// buffer의 시작주소부터 PacdetHeader 만큼의 길이를 복사해 PacketHeader로 해석하면 됩니다.
		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[0]));

		// 헤더에 기록된 패킷크기를 파싱할 수 있어야 합니다. 
		if (dataSize < header.size)
			break; 
		// 위에서 나온 header.size가 헤더 4바이트와 데이터를 포함할것인지 헤더빼고 뒤의 데이터의크기만을 말할것인지
		// 스스로 정해줄 수 있습니다. 그에 맞게 모든 코드를 적용시켜주면되는데
		// 지금 배우는 코드에서는 데이터 + 헤더의 크기를 모두 더한 크기를 size라고 하겠습니다. 

		// 위의 조건문을 통과했다는것은 의도했던 패킷의 크기이상을 받아왔다는 뜻입니다.
		// OnRecvPacket을 호출해 컨텐츠단에서 처리하게 넘겨줍니다.
		OnRecvPacket(&buffer[0], header.size);

		// 이제 처리한 패킷 사이즈 만큼을 기록합니다. 
		processLen += header.size;
	}

	return processLen;
}
