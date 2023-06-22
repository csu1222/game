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

void Session::Send(BYTE* buffer, int32 len)
{
	/*
	������ ���� 
	1) ���� ����?
	2) sendEvent ����? ����? ������? WSASend ��ø?
	- MMORPG�� ���������� Send�� �ѹ��� �ѹ��� ���ʴ�� ȣ���ϴ°� �ƴ϶� ���� ���, �ӼӸ� ���� 
	  �ѹ� �ѹ� �Ͼ�� ���� ��ø�ؼ� Send�� ȣ������� �մϴ�. �׷���� _recvEvent ó�� 
	  �� �ϳ��� ���� ����ϴٺ��� ��ø�� Send ȣ���� ������ �� �����ϴ�. 
	  ������ �ӽ÷� Send�Լ� �ȿ��� �ǽð� �����ϴ¹������ �����غ��ڽ��ϴ�. 
	*/

	// TEMP
	SendEvent* sendEvent = A_new<SendEvent>();
	sendEvent->owner = shared_from_this();		// ADD_REF
	// �ӽ÷� SendEvent�� ���۸� ��� �������̰� �ű⿡ ���������͸� �������ݴϴ�. 
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	/*
	�׻� Register, Process ������ �����߾����ϴ�.���� ���ۿ� ����� sendEvent �� ���ڷ� �޽��ϴ�.
	Send�� ��� ä��, ������ ȹ��, ����ġ ȹ�� ���� �ֱ������� ȣ��Ǵ°� �ƴ϶� Ư����Ȳ�� �ѹ��� ����
	������� ȣ��� �� �ֽ��ϴ�. 
	�׷��� RegisterSend ���� WSASend �Լ��� ������ ȯ�濡�� ���������� �˾ƾ� �ϴµ� ���Ĺ����� ���� 
	�����忡�� ������ ����Ǿ� ���� �ʴٰ� �մϴ�. �׷��� �츮�� ���� ������ ��������� �մϴ�. 
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

	OnDisconnected();	// ������ �ڵ忡�� �����ε�

	SocketUtils::Close(_socket);	// ���ϵ� �ݽ��ϴ�.
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
	case EventType::Send:	// ������ SendEvent�� ���ڷ� �Ѱ�����մϴ�.
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
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
	/*
	Send �Լ����� ���� �ɾ��־����ϴ�. ���� ������� WSASend�� ȣ���� �� �ٵ� �ƹ��� ���� 
	�����忡�� WSASend�� �õ��ϴ��� ���� ���� ���� �õ��� ������� ������ �ϰ� �˴ϴ�. 
	�� ���� IocpCore::Dispatch �ȿ� GetQueuedCompletionStatus �� ȣ�������ٵ� 
	�̺κп����� ���������� �� �� �����ϴ�. IocpCore ������ ���� ���� ������� �ʱ� �����Դϴ�. 
	�׷��� �����͸� ������ ������ �߿��� ���� ������ �� �� �ֽ��ϴ�.

	�� �Ѱ��� WSASend�� ȣ���� ���� Pending ��Ȳ�� �Ǿ��ٴ°��� Ŀ���� SendBuffer�� ���� �ִٴ� �̾߱��Դϴ�.
	�� ��Ȳ���� ������ ���� WSASend�� ��� �о�ִ°� �������� ������ ���� �մϴ�. 
	�츮�� WSASend�� ȣ���Ҷ� WSABUF�� ���빰�� buf�� len�� �Ѱ��ִµ� Pending ��Ȳ������ 
	���ݴ��� Ŀ�������� �� WSABUF �� �Ѱ��� �� ���ٰ� �ص� �������� Ŀ���� ���۷� ���簡 �̷����̴ϴ�.
	��� �����̶� �ڼ����� �˾ƺ��� ���������� �̷��� WSABUF �� ������� �������� �̶�°��� �Ǵٰ� �մϴ�.

	�׸��� WSASend�� ȣ���Ҷ� ���������͸� ���� ������ ȣ���ϴ°ͺ��� �����͸� �ѹ��� ��Ƽ� 
	WSASend�� ȣ���ϴ°� �����ϴ�. ���ڿ����� WSABUF �� �� WSABUF�� ������� �޽��ϴ�.
	��, ��Ƽ� ������ �ֵ��� �Ǿ� �ֽ��ϴ�. 

	���� �۾����� �ڵ�� �̷��� �κе��� �ƽ����ϴ�. ���������� ������ �Ǵ°͵� �ƴϰ� 
	������ �����Ͷ� �Ź� RegisterSend�� ȣ���ϰ� �Ǵµ�
	�׷��� ���� ���� �����͸� ��� ������ �����ϴٰ� ���� ũ�Ⱑ �Ǹ� �ѹ��� ���� �� �ֵ��� ���ָ� 
	�� �� ���ɻ����� �����ϴٰ� �� �� �ֽ��ϴ�. 
	*/

	// ���ӿ��� ���� üũ
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
	// ���� IocpCore->Dispatch �� ���� ProcessSend�� �Ѿ���Դϴ�.
	// �׷��� ���⼭�� ���������� Session�� ��� �����μ� SendEvent�� ��� �ִ°� �ƴϱ� ������ 
	// �ٽ� ���ڷ� �Ѱ��ֵ��� ����� �մϴ�. 
	
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

	// TODO : ������ ������ �κ��ε� �α���� �ڵ�� �����ϰ� OnRecv��� ������ �ʿ��� �����ε��� ����� �Լ��� ȣ���մϴ�. 
	// cout << "Recv Data Len = " << numOfBytes << endl;
	OnRecv(_recvBuffer, numOfBytes);

	// ������ ������ ���� �ٽ� Register�� �̴ϴ�.
	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	// ProcessRecv�� ���������� �����ߴٸ� �� ������ ����ī��Ʈ�� �ٿ��ݴϴ�.
	sendEvent->owner = nullptr;
	A_delete(sendEvent);

	if (numOfBytes == 0)
	{
		// Dispatch�� ����� ���¿��� ���� �����Ͱ� 0�̶�� ������ ����ٴ� �̾߱�
		Disconnect(L"Recv 0");
		return;
	}

	// ������ �ڵ忡�� �����ε�
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
