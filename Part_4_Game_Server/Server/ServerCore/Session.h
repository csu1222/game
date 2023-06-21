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
클라이언트와 서버가 connect, accept 를 통과해 연결된 다음부터는 
Session이 중요한 역할을 하게 되는데 
Recv를 하면서 패킷을 받기시작하는것부터 
서버에서 클라이언트로 다시 데이터를 전송하는 Send 등의 기능을 
이번 시간에 구현해볼것입니다. 
*/

class Session : public IocpObject
{
	// 앞으로 모든 함수들을 public으로 열지는 않을것인데 
	// 그래도 외부에서 데이터를 주고받아야하는 클래스들을 열어줬습니다. 
	friend class Listener;
	friend class IocpCore;
	friend class Service;
public:
	Session();
	virtual ~Session();

public:
	/* 연결 관련 함수 */
	void				Disconnect(const WCHAR* cause);
	
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* 정보 관련 함수 */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	
	// 이전에 누락했던 연결여부를 확인하는 함수
	// 이 함수의 반환값을 통해 계속해서 패킷을 Recv할지를 판단
	bool				IsConnected() { return _connected; }
	// shared_ptr로 자기자신을 반환하는 헬퍼 함수 
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
	/* 인터페이스 구현 */
	// 컨텐츠 단에서 인터페이스 구현쪽 함수에 접근할 필요는 없으니 private로 막았습니다. 
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 전송 관련 함수 */
	// Listener 에서와 같은 형태로 다른 비동기 IO 함수들을 작업하겠습니다. 
	// 현재 프로젝트에서는 RegisterConnect 는 별의미 없이 곧바로 Connect를 호출하지만 
	// 이후 컨텐츠 단에서 사용할지도 모르기때문에 만들었습니다. 
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 오버로딩해 사용할 함수 */
	
	// 접속, 끊김, 받고, 보내고에 대한 행동들을 함수로 호출할 준비를 해두고 나중에 필요할때 오버로딩해서 사용합니다.
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	char				_recvBuffer[1000] = {};

private:
	// session 내부에서도 자신을 물고있는 Service를 알아야 접속하거나 끊을수 있습니다. 
	// 순환 문제 때문에 weak_ptr로 만듭니다. 
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	// 멀티스레드 환경에서 사용할것이기 때문에 Lock도 사용합니다. 
	USE_LOCK;

	/* 수신 관련 TODO */

	/* 송신 관련 TODO */

private:
	/* IocpEvent 재사용 관련 */
	RecvEvent			_recvEvent;
};