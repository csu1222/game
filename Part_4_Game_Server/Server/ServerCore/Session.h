#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"


/*
--------------------
	Session 선언부
--------------------
*/

class Service;

/*
이번 시간은 Send를 구현해 볼 것인데 Send는 Recv와 다른점이 있습니다. 
Recv는 RegisterRecv를 걸어놓고 패킷이 들어오는걸 기다리는 방식이었다면 
Send는 보낼 데이터가 준비 됐으면 기다리지 않고 바로 호출해주는 방식입니다. 
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
	/* 외부에서 사용 */
	
	// Send의 인자 BYTE는 unsigned_char를 바꿔말한겁니다. 
	void				Send(BYTE* buffer, int32 len);
	void				Disconnect(const WCHAR* cause);
	
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* 정보 관련 함수 */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
	/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 전송 관련 함수 */
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 오버로딩해 사용할 함수 */
	
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	// TEMP
	BYTE				_recvBuffer[1000] = {};

	// Send용으로 임시버퍼를 또 만들었습니다. 
	/*
	_sendBuffer 를 이렇게 만들면 문제가 Send를 중첩해서 호출할 때도 있을텐데 
	그러면 보낼 데이터가 손실나지 않도록 해줘야 합니다. 
	순환 버퍼(Circular Buffer) 방식으로는 Send를 할때 그 데이터를 _sendBuffer에 복사하고 
	다음번 Send하는 스레드는 이전 데이터 뒤에 이어서 복사하는식으로 쭉이어지다가 
	_sendBuffer를 다 사용하면 다시 가장앞으로 돌아와 데이터를 이어 복사하는 방식이 있습니다. 
	이 방식의 문제는 한번에 너무 많은 Send를 호출하게 되면 각 호출마다 데이터를 복사하는 비용이 
	많이 든다는 것입니다.
	그래서 이번 시간에는 사용하지 않고 
	IocpEvent 클래스->SencEvent 안에 직접 버퍼를 들고 있도록 합니다. 
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

	/* 수신 관련 TODO */

	/* 송신 관련 TODO */

private:
	/* IocpEvent 재사용 관련 */
	RecvEvent			_recvEvent;
};