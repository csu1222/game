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
�̹� �ð��� Send�� ������ �� ���ε� Send�� Recv�� �ٸ����� �ֽ��ϴ�. 
Recv�� RegisterRecv�� �ɾ���� ��Ŷ�� �����°� ��ٸ��� ����̾��ٸ� 
Send�� ���� �����Ͱ� �غ� ������ ��ٸ��� �ʰ� �ٷ� ȣ�����ִ� ����Դϴ�. 
*/

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
public:
	Session();
	virtual ~Session();

public:
	/* �ܺο��� ��� */
	
	// Send�� ���� BYTE�� unsigned_char�� �ٲ㸻�Ѱ̴ϴ�. 
	void				Send(BYTE* buffer, int32 len);
	void				Disconnect(const WCHAR* cause);
	
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* ���� ���� �Լ� */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
	/* �������̽� ���� */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� �Լ� */
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	/* ������ �ڵ忡�� �����ε��� ����� �Լ� */
	
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	// TEMP
	BYTE				_recvBuffer[1000] = {};

	// Send������ �ӽù��۸� �� ��������ϴ�. 
	/*
	_sendBuffer �� �̷��� ����� ������ Send�� ��ø�ؼ� ȣ���� ���� �����ٵ� 
	�׷��� ���� �����Ͱ� �սǳ��� �ʵ��� ����� �մϴ�. 
	��ȯ ����(Circular Buffer) ������δ� Send�� �Ҷ� �� �����͸� _sendBuffer�� �����ϰ� 
	������ Send�ϴ� ������� ���� ������ �ڿ� �̾ �����ϴ½����� ���̾����ٰ� 
	_sendBuffer�� �� ����ϸ� �ٽ� ��������� ���ƿ� �����͸� �̾� �����ϴ� ����� �ֽ��ϴ�. 
	�� ����� ������ �ѹ��� �ʹ� ���� Send�� ȣ���ϰ� �Ǹ� �� ȣ�⸶�� �����͸� �����ϴ� ����� 
	���� ��ٴ� ���Դϴ�.
	�׷��� �̹� �ð����� ������� �ʰ� 
	IocpEvent Ŭ����->SencEvent �ȿ� ���� ���۸� ��� �ֵ��� �մϴ�. 
	*/
	//char				_sendBuffer[1000] = {};
	//int32				_sendLen = 0;

private:
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	USE_LOCK;

	/* ���� ���� TODO */

	/* �۽� ���� TODO */

private:
	/* IocpEvent ���� ���� */
	RecvEvent			_recvEvent;
};