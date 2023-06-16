#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/*
--------------------
	Session �����
--------------------
*/

/*
�����ϴ� Ŭ�󸶴� ����, �ּ� ��� ������ �����͵��� ��� 
�� Session���� �����ϰ� �˴ϴ�. 
�׸��� �� Session �� IocpCore�� ����� ����մϴ�. 
*/

// �̹� �ǽ������� ���� �׽�Ʈ�� �Ұ��̱⶧���� �����ϰ� ����ϴ�.
class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	/* ���� ���� �Լ� */
	void		SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress	GetAddress() { return _netAddress; }
	SOCKET		GetSocket() { return _socket; }

public:
	/* �������̽� ���� */
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	// TEMP : �ӽ÷� Recv���۸� ����ϴ�. ���߿��� �����ɰ��Դϴ�.
	char _recvBuffer[1000] = {};

private:
	SOCKET			_socket = INVALID_SOCKET;
	NetAddress		_netAddress = {};
	// ���� ������ ���ӿ��θ� ��� �ֽ��ϴ�. 
	Atomic<bool>	_connected = false;
};

