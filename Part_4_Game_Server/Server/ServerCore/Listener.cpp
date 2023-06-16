#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"


/*
--------------------
	Listener ���Ǻ�
--------------------
*/
Listener::~Listener()
{
	// �Ҹ� ������ Listener�� ����ִ� ������ �ݾ��ݴϴ�. 
	SocketUtils::Close(_socket);

	// �ݺ��������鼭 ����ִ� AcceptEvent �鵵 �����մϴ�.
	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO 

		A_delete(acceptEvent);
	}

}

// Listener�� ���� ���� �� �������� Accept�� ȣ���մϴ�.
bool Listener::StartAccept(NetAddress netAddress)
{
	// �ϴ� Listener�� ������ ����ϴ�. 
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	// �������� Listener�� CP�� ����մϴ�. 
	if (GIocpCore.Register(this) == false)
		return false;

	// ���� �ɼ��� �ּ� ���� �����ϰ� �ؼ� �ּҰ� ���ĵ� ���۰����ϰ� ���ݴϴ�. 
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	// Linger��� �ɼ��� ���� ������ ���۵��� ������ �������� ������ �ִ� �����͸� ��ö� 
	// �����ϰڴ°�? �����Ѵٸ� ������� �ð����� �����Ұ������� ���� �ɼ��Դϴ�. 
	// ���⼭�� �Ѵ� 0, 0 ���� �־� ������ ����� �׳� �����͸� �� �����°ɷ� �Ͽ����ϴ�. 
	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	// ������� ���Ͽɼ��� �� �������� Bind�� listenSocket�� ���� �ڽ��� �ּҸ� �����ϴ�. 
	if (SocketUtils::Bind(_socket, netAddress) == false)
		return false;

	if(SocketUtils::Listen(_socket) == false)
		return false;


	//������� ���Ͽɼ�, Bind, Listen ���� ������ AcceptEx�� ȣ�⿹���� �����Դϴ�.
	//RegisterAccept �� ����ؼ� AcceptEx �� �����մϴ�. ���� �������� �����ϸ� �Ϸ� ������ �߰� �ɰ̴ϴ�. 
	//�Ϸ� ������ CP�� �����ϴ� Worker �����忡�� �����Ұ̴ϴ�.
	//�׸��� �ѹ��� ���ӿ�û�� ������ Accept ������ �ϳ����̸� ���� ��ĥ���� �ֱ⶧���� for������ 
	//������ ������ �ɾ������ �����ϴ�. �ϴ� ������ �ϳ��� �����մϴ�.
	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = A_new<AcceptEvent>();
		// �� �ݺ��� �ȿ��� ���� acceptEvent�� _acceptEvents ��� ���Ϳ����� �����ϰ� �ؼ� 
		// ���߿� �����ϱ� ������ �մϴ�. �Ҹ��� ȣ�⶧ �˾Ƽ� �������ݴϴ�.
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return false;
}

// �� �Լ��� ���Ǿƴϰ� �׳� �� lisnteSocket�� �ݾ��ݴϴ�.
void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

// _socket�� HANDLE�� ĳ������ ��ȯ
HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

// IocpCore::Dispatch ���� GetQueuedCompletionStatus �� ������� IocpEvent->Dispatch�� ȣ���Ѱ� �̰����� �ɴϴ�.
void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// ������� �׽�Ʈ���� Event Ÿ���� Accept ���̴ϱ� ����üũ���ݴϴ�.
	ASSERT_CRASH(iocpEvent->GetType() == EventType::Accept);

	// iocpEvent�� AcceptEvent�� �������ְڽ��ϴ�. 
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	
	// ���� �� AcceptEvent�� �����ϱ����� ProcessAccept �� ȣ���մϴ�
	ProcessAccept(acceptEvent);
}

// AcceptEx�� �������ִ� �Լ� ��, ���������� AcceptEx�� ȣ�����ݴϴ�.
// ���⼭ �����ص� AcceptEx�� ������ �������ְԵǸ� Dispatch�� �Ѿ�̴ϴ�.
void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	/*
	AcceptEx�� �޴� ���ڰ� ������ Ŭ���̾�Ʈ�� ���� ���ڰ� �����ϴ�.
	�� ���ڸ� ���� �� �Լ� ������ ����⺸�ٴ� Session�̶�� Ŭ���̾�Ʈ�� ���� �����͵��� ��Ƶ� 
	��ü���� ������ ����ϴ°� �����ϴ�. 
	���߿��� �̸� �غ�� ������ Ǯ���ؿͼ� ����ص� ������, ������ ���� �׽�Ʈ������ �غ����̴�
	�����ϰ� ���⼭ �����߽��ϴ�.
	�̹��ð��� �߰��� AcceptEvent Ŭ�������� ������ ����Ҽ� �ֵ��� ��������ϴ�. 
	�ֳĸ� Dispatch�� ProsessAccept ���� AcceptEvent�� �޾� ������ ���ٵ� 
	� ���ǰ� �����Ǿ��ִ����� Ȯ���Ҽ� �ֵ��� �մϴ�.
	*/
	Session* session = A_new<Session>();
	acceptEvent->Init();
	acceptEvent->SetSession(session);

	/*
	AcceptEx�� ���ڸ��
	1) listener Socket
	2) AcceptEx ȣ���� clientSocket�� �� ����
	3) lpOutputBuffer ó�� ����Ǹ鼭 ������ �����ּ� �� Ŭ���̾�Ʈ�� �����ּҸ� ������ ����
	4) dwReceiveDataLength ���� ���� �����ּ�, Ŭ���� �����ּҸ� ������ ����� ���ÿ� ������ �������� ũ��
		0�� ������ ����� ���ÿ� �������� �ʰ� �ٷ� AcceptEx�� �Ϸ��մϴ�.
	5) dwLocalAddressLength �����ּҸ� ���� ����� ����Ʈ ���Դϴ�. ���� ������� ������������
		SOCKADDR_IN ���� 16����Ʈ �̻��̾�� �մϴ�. 
	6) dwRemoteAddressLength �����ּҸ� ���� ����� ����Ʈ ���Դϴ�. �;� �� ���� ���� �����ϴ�.
	7) lpdwBytesReceived �����ϸ鼭 ���� �������� ����Ʈ ���� ������ �����Դϴ�. DWORD* Ÿ���Դϴ�.
	8) lpOverlapped ��û�� ó���Ҷ� ���Ǵ� LPOVERLAPPEC ����ü�Դϴ�. �츮�� IocpEvent�� ĳ������ �־��ָ�˴ϴ�. 
	*/
	DWORD bytesReceived = 0;
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		// ���������� �����ڵ带 ���ϴ�.
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// Pending ���°� �ƴ϶�� ������ �ִ� ��Ȳ�Դϴ�.
			// �̰�쿡�� RegisterAccept ȣ���ϴ��� ���� ��Ȳ�̴� ���� �ٽ� �ɾ���� �մϴ�.
			RegisterAccept(acceptEvent);
		}
	}
}


void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	// AcceptEvent�� �����ص״� ������ �����ϴ�. 
	Session* session = acceptEvent->GetSession();

	// SetUpdateAcceptSocket �̶�� �Լ��� ���� ���ϰ� listen ������ �ɼ��� �Ȱ��� �����ݴϴ�.
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		// ���������� �����ϱ����� �� RegisterAccept�� ȣ�����༭ ���� ������ ���ݴϴ�. 
		RegisterAccept(acceptEvent);
		return;
	}

	// �����͸� �����غ��ڽ��ϴ�. getpeername �Լ��� ���� sockAddress�� �ּҸ� ���������ֽ��ϴ�. 
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	// ������ �ּҸ� NetAddress ��ü�� ����� �����մϴ�. 
	session->SetNetAddress(NetAddress(sockAddress));

	// ������� ������ ���������� ����Ǿ��ٰ� �� �� �ֽ��ϴ�.
	cout << "Client Connected!" << endl;

	// TODO

	RegisterAccept(acceptEvent);

	// �� ������� ���� �ѹ� Ŭ���̾�Ʈ�� �����ϰ� ������ ���ǰ� IocpEvent�� ��� �����ϰ� �ֽ��ϴ�. 
}
