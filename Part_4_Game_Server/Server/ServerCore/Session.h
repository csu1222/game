#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"


/*
--------------------
	Session �����
--------------------
*/

class Service;

/*
Ŭ���̾�Ʈ�� ������ connect, accept �� ����� ����� �������ʹ� 
Session�� �߿��� ������ �ϰ� �Ǵµ� 
Recv�� �ϸ鼭 ��Ŷ�� �ޱ�����ϴ°ͺ��� 
�������� Ŭ���̾�Ʈ�� �ٽ� �����͸� �����ϴ� Send ���� ����� 
�̹� �ð��� �����غ����Դϴ�. 
*/

class Session : public IocpObject
{
	// ������ ��� �Լ����� public���� ������ �������ε� 
	// �׷��� �ܺο��� �����͸� �ְ�޾ƾ��ϴ� Ŭ�������� ��������ϴ�. 
	friend class Listener;
	friend class IocpCore;
	friend class Service;
public:
	Session();
	virtual ~Session();

public:
	/* ���� ���� �Լ� */
	void				Disconnect(const WCHAR* cause);
	
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* ���� ���� �Լ� */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	
	// ������ �����ߴ� ���Ῡ�θ� Ȯ���ϴ� �Լ�
	// �� �Լ��� ��ȯ���� ���� ����ؼ� ��Ŷ�� Recv������ �Ǵ�
	bool				IsConnected() { return _connected; }
	// shared_ptr�� �ڱ��ڽ��� ��ȯ�ϴ� ���� �Լ� 
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
	/* �������̽� ���� */
	// ������ �ܿ��� �������̽� ������ �Լ��� ������ �ʿ�� ������ private�� ���ҽ��ϴ�. 
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� �Լ� */
	// Listener ������ ���� ���·� �ٸ� �񵿱� IO �Լ����� �۾��ϰڽ��ϴ�. 
	// ���� ������Ʈ������ RegisterConnect �� ���ǹ� ���� ��ٷ� Connect�� ȣ�������� 
	// ���� ������ �ܿ��� ��������� �𸣱⶧���� ��������ϴ�. 
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	/* ������ �ڵ忡�� �����ε��� ����� �Լ� */
	
	// ����, ����, �ް�, ������ ���� �ൿ���� �Լ��� ȣ���� �غ� �صΰ� ���߿� �ʿ��Ҷ� �����ε��ؼ� ����մϴ�.
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	char				_recvBuffer[1000] = {};

private:
	// session ���ο����� �ڽ��� �����ִ� Service�� �˾ƾ� �����ϰų� ������ �ֽ��ϴ�. 
	// ��ȯ ���� ������ weak_ptr�� ����ϴ�. 
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	// ��Ƽ������ ȯ�濡�� ����Ұ��̱� ������ Lock�� ����մϴ�. 
	USE_LOCK;

	/* ���� ���� TODO */

	/* �۽� ���� TODO */

private:
	/* IocpEvent ���� ���� */
	RecvEvent			_recvEvent;
};