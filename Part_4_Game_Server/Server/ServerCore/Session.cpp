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
	// exchage�� ���ڷ� _connected�� ���� �ٲ�� ��ȯ�� ���� ������ �ִ� ���� ��ȯ�մϴ�.
	// �׷��� �Ʒ� ���ǹ��� ���̶�°��� �̹� _connected�� ���� false ���ٴ� ���Դϴ�. 
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnet : " << cause << endl;

	OnDisconnected();	// ������ �ڵ忡�� �����ε�

	SocketUtils::Close(_socket);	// ���ϵ� �ݽ��ϴ�.
	GetService()->ReleaseSession(GetSessionRef());	// Release Ref

	// ���� Session�� ���� ī��Ʈ�� ���������� 0�� �Ǹ鼭 �Ҹ��մϴ�.
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// ������� ���Դٴ°� IocpCore::Dispatch�� �� �Ϸ�ǰ� ����ߴ� �񵿱� IO �Լ��� �Ϸ�Ǿ��ٴ� �̾߱�

	// IocpEvent�� Ÿ�Կ� ���� �ϴ����� �����ϴ�. �̹� �ð����� �ϴ� Recv�� �����մϴ�.
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
	// ���ӿ��� ���� üũ
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	// _recvBuffer�� ���� �ӽ÷� ����ϴ� ���̰� ���߿� ������ ����
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);	
	wsaBuf.len = len32(_recvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	// owner�� �����ϴ°����� ��ǻ� owner�� Ref Count�� 1�ø���
	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();


	/*
	WSARecv�� ������ OVERLAPPED ����ü�� �ش��ϴ� IocpEvent ��ü, Session������ RecvEvent�� 
	���� �Լ����� �Ź� ���� ����� �ִ� ����� �ְ� �ƴϸ�
	IocpEvent���� �� ���Ǹ��� �Ҵ�Ǿ� �����Ұ��̱� ������ ��������� ��� �ְ� �װ� �Ѱ��ִ� ����� �ֽ��ϴ�.
	������ ���ο� ��� ������ Event�� ��� �ִ� ������� �غ��ڽ��ϴ�. 
	*/
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		// WSARecv �� ��¥ �����ߴ����� üũ
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// ��� ���°� �ƴϸ� ���� ���� ������ �����
			// HandleError �Լ��� � ���������� ����ϰ� ���� ������ ����ī��Ʈ�� ���Դϴ�.
			HandleError(errorCode); 
			_recvEvent.owner = nullptr;
		}
		// �����ڵ尡 ��������̸� �׳� ���� ���� �����Ͱ� ���ٴ� ��
		// �����Ͱ� ���ö� ���� ����(Dispatch) 
	}
}

void Session::RegisterSend()
{
}

void Session::ProcessConnect()
{
	_connected.store(true);

	// ���� ��� : ���ڵ忡 �ͼ��� ���񽺿� ������ ������ �������ִ°̴ϴ�. 
	GetService()->AddSession(GetSessionRef());

	// ������ �ڵ忡�� �����ε��� ����
	OnConnected();

	// ���� ��� : ���� �� ���⼭ ���� Recv�� Iocp�� ����� ���ݴϴ�. 
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	// ���̻� WSARecv �� ������ �ɸ� ���°� �ƴϴ� �� ������ ����ī��Ʈ�� �ٿ��ݴϴ�. 
	_recvEvent.owner = nullptr;

	if (numOfBytes == 0)
	{
		// Dispatch�� ����� ���¿��� ���� �����Ͱ� 0�̶�� ������ ����ٴ� �̾߱�
		Disconnect(L"Recv 0");
		return;
	}

	// TODO 
	cout << "Recv Data Len = " << numOfBytes << endl;

	// ������ ������ ���� �ٽ� Register�� �̴ϴ�.
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
