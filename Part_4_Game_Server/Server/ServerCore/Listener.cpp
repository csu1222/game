#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"


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
bool Listener::StartAccept(ServerServiceRef service)
{
	// _service�� ä���ݴϴ�. 
	_service = service;
	if (_service == nullptr)
		return false;

	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;
 
	/*
	���ݱ����� �ӽ÷� IocpCore ��ü�� �׳� extern ���� ����� �־��µ� �̺κ��� �������ְڽ��ϴ�. 
	IocpCore Ŭ�������� ������ ������ ������ _service���� ���� ����� �� �ֽ��ϴ�. 
	�׸��� IocpCore->Register() �� ���ڷ� �׳� this �� �Ѱ��ָ� �ȵǰ� 
	shared_from_this() �� �Ѱ�����մϴ� 
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

	// ���� acceptCount�� ServerService�� maxSessionCount ������ �ְڽ��ϴ�. 
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
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);

	// iocpEvent�� AcceptEvent�� �������ְڽ��ϴ�. 
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	
	// ���� �� AcceptEvent�� �����ϱ����� ProcessAccept �� ȣ���մϴ�
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	// ���� MakeShared �� �������ִ� session ��ü�� _service->CreateSession() ���� ��ü�մϴ�.
	// CreateSession() ������ ��ü �����Ӹ��ƴ϶� �� ������ CP�� ��ϱ��� ���ݴϴ�. 
	SessionRef session = _service->CreateSession();
	acceptEvent->Init();
	acceptEvent->session = session;

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
	SessionRef session = acceptEvent->session;

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
