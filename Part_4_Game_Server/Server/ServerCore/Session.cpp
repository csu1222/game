#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"
#include "IocpEvent.h"

/*
--------------------
	Session ���Ǻ� 
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

void Session::Send(BYTE* buffer, int32 len)
{
	// TEMP
	SendEvent* sendEvent = A_new<SendEvent>();
	sendEvent->owner = shared_from_this();		// ADD_REF
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	WRITE_LOCK;
	RegisterSend(sendEvent);

}

bool Session::Connect()
{
	/*
	Connect �� �״�� ������ �ϴ� �Լ� �Դϴ�. ���� ���忡���� Accept�� ��ٸ��� �ִٰ� 
	������ Ŭ�� ���� ����� �ϸ� �Ǳ� ������ ��� Connect�� �� �ʿ����� ���� �� �� �� �ִµ�
	������ �ѹ� ����ߴ� ���� �ֽ��ϴ�. ������ ���ϼ����� �����Ҷ��� �ְ� ���߼����� ������ ������ 
	�����ϴ� ��쵵 �ִµ� ���� �����϶� �������� ������ �Ҷ� �� �Լ��� �ʿ������ϴ�.

	*/
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	/*
	������ ������ �׳� Session�� ��� �ִ� ������ ������ �ݾ��ְ� �ֽ��ϴ�. 
	�� �̰� ������� ���ٴ� �̷��� ������ �ݾ��൵ ������ ������ �մϴ�.
	�����ε� �̷�������� ������ ���� ��쵵 �ֱ���ѵ� 
	�츮�� SocketUtils ���� ��Ÿ�ӿ� �޾���� �Լ����� DisconnectEx ��°͵� �ֽ��ϴ�.
	�񵿱� IO �Լ��� �ϳ��ε� �̰� ����ϸ� ȣ���ϸ� IOCP�� ������ �س��� 
	ó���� �غ� �Ǹ� �����忡�� ó�����ִ� ����Դϴ�. 
	
	�׷��� DisconnectEx�� ����ϴ� ������ �����ΰ��ϸ� 
	������ Session ������, �Ҹ���, �׸��� ���� Disconnect���� ������ ����� �ݾ��ְ� �ϴµ�
	��� �� �۾��� �δ��� ���� ���� �۾��̶�� �մϴ�. �׷���ŭ �ѹ� ���� ������ �����Ѵٸ� 
	���ɻ� ���� �̵��� �ɰ̴ϴ�. 
	���ɻ����ε� ���� ��¥�� Iocp�� �̿��ϰ� �ִµ� ���� DisconnectEx�� ������� ���� ������ 
	�����ϱ� �� ������� ������ڽ��ϴ�. 
	*/
	
	// _connected�� ���� false �� �־��ִµ� �̹� ���� false������ ��ȯ�� false�� ��ȯ�˴ϴ�.
	// ��, �� Disconnect �Լ��� �ѹ��� ȣ���ϵ��� �Ѱ��Դϴ�. 
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnet : " << cause << endl;

	OnDisconnected();	// ������ �ڵ忡�� �������̵�

	// SocketUtils::Close(_socket);	// ���ϵ� �ݽ��ϴ�.
	GetService()->ReleaseSession(GetSessionRef());	// Release Ref
	
	// SocketUtils::Close ��� RegisterDisconnect �� ȣ���մϴ�. 
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
	case EventType::Send:	// ������ SendEvent�� ���ڷ� �Ѱ�����մϴ�.
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
		break;
	}
}

bool Session::RegisterConnect()
{
	// �̹� ����� �������� üũ 
	if (IsConnected())
		return false;

	// ���� �� ������ ���� ������ Ÿ���� Ŭ���̾�Ʈ������ üũ�մϴ�. 
	// Ŭ���̾�ƮŸ���� �ٸ� ������ �����ϰ� ���� Ÿ���� ������ ������ ������ �õ��ϴ°��Դϴ�. 
	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	// ������ �ɼǵ��� �������ݴϴ�. 
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;
	
	// Listener�� ���� �ɼ��� �����Ҷ��� ���� �ʾҴµ� Ŭ�����忡���� �ƹ� Address, Port �� 
	// ����ϵ��� �߽��ϴ�. 
	if (SocketUtils::BindAnyAddress(_socket, 0/*0�� �ָ� ���� ��Ʈ �ƹ��ų� ����*/) == false)
		return false;
	
	// Connect�� ���ÿ� ������ �Ͼ�� �۾��� �ƴϱ� ������ ���� ��� ������ ��� �־ �˴ϴ�.
	_connectEvent.Init();
	_connectEvent.owner = shared_from_this();	// ADD_REF

	// ConnectEx �� �ʿ��� ���ڵ�
	DWORD numOfBytes = 0;	// ���Ӱ� ���ÿ� ���� �������� ũ�⸦ �������Դϴ�.
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr(); // ���� ���� ��������

	if (SOCKET_ERROR == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		// Pending�������� ���� üũ
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// Pending ���°� �ƴ϶�� ��¥�� ����
			_connectEvent.owner = nullptr;	// RELEASE_REF
			return false;
		}
		// Pending ���¶�� �� �� ���
	}
	return true;
}

bool Session::RegisterDisconnect()
{
	// �ݺ��Ǵ� �����Դϴ�.
	// _disconnectEvent �ʱ�ȭ, �ڽ��� ����� ADD_REF
	_disconnectEvent.Init();
	_disconnectEvent.owner = shared_from_this();	// ADD_REF

	/*
	DisConnectEx �Լ��� �����Դϴ�. 
	1) ����
	2) OVERLAPPED �� ������ �츮�� IocpEvent ��ü�� �ָ�˴ϴ�.
	3) �Լ� ȣ�⿡ ���� �÷��� �Դϴ�. 0�� �ָ� �ƹ��� �÷��װ� �������� �ʰ� 
	   ���� ó�� TF_REUSE_SOCKET �� �ָ� DisconnectEx ȣ�� �� �ٽ� AcceptEx�� ConnectEx����
	   �� ������ ����� �� �ֽ��ϴ�.
	4) reserved �� ����Ѵٰ� �ϴµ� �� 0�� ����մϴ�. �ٸ� ���� �ָ� �߸��� ���ڸ� ��ٴ� ������ ��ϴ�.
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
	// ���ӿ��� ���� üũ
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	// wsaBuf.buf �� _recvBuffer�� ���ۺ��� �����ʰ� _writePos ���� ���� ����
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	// len�� ���� ������ ����� �ƴ� ������ ���������� ���մϴ�. 
	wsaBuf.len = _recvBuffer.FreeSize();

	// Ŀ�ο��� ������ ������ ������ ������� numOfBytes�� �޾��ٰ��Դϴ�. 
	DWORD numOfBytes = 0;
	DWORD flags = 0;

	// owner�� �����ϴ°����� ��ǻ� owner�� Ref Count�� 1�ø���
	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();


	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		// WSARecv �� ��¥ �����ߴ����� üũ
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode); 
			_recvEvent.owner = nullptr;
		}	}
}

void Session::RegisterSend(SendEvent* sendEvent)
{
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)sendEvent->buffer.data();
	wsaBuf.len = (ULONG)sendEvent->buffer.size();

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, OUT & numOfBytes, 0, sendEvent, nullptr))
	{
		// ��¥ �������� PENDING �������� üũ 
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent->owner = nullptr;
			//�ǽð����� �����ߴ� sendEvent�� ���� �������� ����
			A_delete(sendEvent);
		}
	}
	
}

void Session::ProcessConnect()
{
	/*
	�� ProcessConnect �Լ��� �ΰ��� �뵵�� ����ϰ� �˴ϴ�. 
	Listener::ProcessAccept �Լ����� ȣ��Ǵ� �� ������ Ŭ�� �����Ҷ� �� ����ϰ� 
	�� ������ Ŭ���̾�Ʈ �����μ� �ٸ� ������ �����Ϸ��� �Ҷ��� ���˴ϴ�. 
	*/

	_connectEvent.owner = nullptr;	// RELEASE_REF

	_connected.store(true);

	// ���� ��� : ���ڵ忡 �ͼ��� ���񽺿� ������ ������ �������ִ°̴ϴ�. 
	GetService()->AddSession(GetSessionRef());

	// ������ �ڵ忡�� �������̵��� ����
	OnConnected();
		
	// ���� ��� : ���� �� ���⼭ ���� Recv�� Iocp�� ����� ���ݴϴ�. 
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	// ���⼭�� ���� �����Ŀ� �����ε� ��� ������ ����ڿ��� ���� �� ���̾��� 
	// ������ ����ī��Ʈ�� ���̰� ��ġ�ڽ��ϴ�. 
	_disconnectEvent.owner = nullptr;	// RELEASE_REF
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr;

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	// ���� ���� ���Դٴ°��� IocpCore::Dispatch ���� ó���� �Ǿ� 
	// _recvBuffer�� �����Ͱ� ���� �Ǿ��ٴ� ���Դϴ�.
	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		// OnWrite�� ������ ���� ���� ������ ���� ó���� �Ĳ��� ���ݴϴ�. 
		Disconnect(L"OnWrite Overflow");
		return;
	}

	// ������ ���ݱ��� write �� ��ŭ�� ������ ����� �޽��ϴ�. 
	int32 dataSize = _recvBuffer.DataSize();

	// _readPos ���� ���� ������ ��ŭ�� ���ڷ� ������ ������ �Ѱ��༭ ó���� ���ְ� �ɰ̴ϴ�. 
	// �׽�Ʈ �ڵ忡���� �����͸� ó���� �� ġ�� �� ����� �ֿܼ� ������ְ� �ֽ��ϴ�. 
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);

	// OnRecv���� ���� �����͸� ó���ߴٴ°��� Read�� �ߴٴ� �� �˴ϴ�. 
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
	{
		// processLen ���� �����̰ų� dataSize ���� ū���� ���� �ȵǴ� ��Ȳ�̰�,
		// ���������� _recvBuffer.OnRead(processLen) ���� _readPos�� ���� �۾��� ���� �ߴٸ� ������ �ִ� ��Ȳ�Դϴ�. 
		Disconnect(L"OnRead Overflow");
		return;
	}

	// Read, Write�� �� ������ Ŀ�� ������ ���ݴϴ�.
	_recvBuffer.Clean();

	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	sendEvent->owner = nullptr;
	A_delete(sendEvent);

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	// ������ �ڵ忡�� �������̵�
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
