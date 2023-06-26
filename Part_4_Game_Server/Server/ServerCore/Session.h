#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"


/*
--------------------
	Session 선언부
--------------------
*/

class Service;

/*
이번 시간은 RecvBuffer 클래스를 만들고 Session에 적용해봅니다. 
*/

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
	
	// 버퍼 사이즈를 하드 코딩하기 보다는 enum으로 관리합니다. 
	enum
	{
		BUFFER_SIZE = 0x10000, // 64kb
	};

public:
	Session();
	virtual ~Session();

public:
	/* 외부에서 사용 */
	void				Send(BYTE* buffer, int32 len);

	// Connect 함수 추가 
	bool				Connect();
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
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 오버라이딩 사용할 함수 */
	
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

private:
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	USE_LOCK;

	/* 수신 관련 TODO */
	RecvBuffer			_recvBuffer;

	/* 송신 관련 TODO */

private:
	/* IocpEvent 재사용 관련 */

	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
};