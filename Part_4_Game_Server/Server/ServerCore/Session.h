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

	// 매번 버퍼를 복사해 Send하는게 아니라 SendBuffer 객체를 참조해 Send하도록 수정
	void					Send(SendBufferRef sendBuffer);

	// Connect 함수 추가 
	bool					Connect();
	void					Disconnect(const WCHAR* cause);
	
	shared_ptr<Service>		GetService() { return _service.lock(); }
	void					SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* 정보 관련 함수 */
	void					SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress				GetAddress() { return _netAddress; }
	SOCKET					GetSocket() { return _socket; }
	
	bool					IsConnected() { return _connected; }
	SessionRef				GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
	/* 인터페이스 구현 */
	virtual HANDLE			GetHandle() override;
	virtual void			Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 전송 관련 함수 */
	bool					RegisterConnect();
	bool					RegisterDisconnect();
	void					RegisterRecv();
	// SendEvent를 멤버 변수로 사용하기 시작하면서 인자로 받아줄 필요가 없습니다.
	void					RegisterSend();

	void					ProcessConnect();
	void					ProcessDisconnect();
	void					ProcessRecv(int32 numOfBytes);
	// SendEvent를 멤버 변수로 사용하기 시작하면서 인자로 받아줄 필요가 없습니다.
	void					ProcessSend(int32 numOfBytes);

	void					HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 오버라이딩 사용할 함수 */
	
	virtual void			OnConnected() {}
	virtual int32			OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void			OnSend(int32 len) { }
	virtual void			OnDisconnected() { }

private:
	weak_ptr<Service>		_service;
	SOCKET					_socket = INVALID_SOCKET;
	NetAddress				_netAddress = {};
	Atomic<bool>			_connected = false;

private:
	USE_LOCK;

	/* 수신 관련 TODO */
	RecvBuffer				_recvBuffer;

	/* 송신 관련 TODO */
	// 이미 RegisterSend가 걸린 상태라 밀렸다면 이 큐로 
	Queue<SendBufferRef>	_sendQueue;
	// 현재 RegisterSend가 걸린 상태인지를 알기 위한 플래그
	Atomic<bool>			_sendRegistered = false;

private:
	/* IocpEvent 재사용 관련 */

	ConnectEvent			_connectEvent;
	DisconnectEvent			_disconnectEvent;
	RecvEvent				_recvEvent;
	// SendEvent도 멤버 변수로 사용할 수 있게 구조를 바꿧습니다.
	SendEvent				_sendEvent;
};