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


/*
--------------------------
	PacketSession 선언부
--------------------------
*/

// PacketSession은 받는쪽과 관련이 있습니다.
// 받는 함수인 ProcessRecv 안에서 받은데이터는 _recvBuffer에 여러 정보들과 함께 받아져 
// 있을것입니다. 일단 받은 이 버퍼를 컨텐츠쪽으로 연결해주는 OnRecv를 읽기 시작할 위치의 
// 주소 ReadPos() 와 받은 데이터 크기를 함께 건내주게 됩니다. 
// 컨텐츠단 역할을 하는 GameSession에서는 OnRecv로 받은 데이터들을 처리해주게 되는데 
// 반환 으로 얼마나 처리했는지를 반환합니다. (현재는 받은데이터 모두를 처리했다고 치고 크기 전부를 반환)
// 그런데 나중에 제대로 MMORPG를 만들게 되면 TCP의 특성상 받은 모든 패킷이 한번에 전송되었다고 보장할 수 없었습니다. 
// 100byte 를 받아야 하는데 어떤 이유로 20byte만을 받게 될 수도 있었습니다. 
// 그렇기 때문에 받은 데이터(패킷)이 온전히 왔는지를 체크할 수 있는 수단이 필요한것이고 
// 그를 위해 우리만의 프로토콜을 정해줘야 합니다. 
// 쉽게 생각하면 데이터중에서 어떤 특정 문자가 나오면 여기까지가 바운더리라고 정하면 되지않을까? 
// 생각할 수 있는데 그러기 어려운 이유는 
// 만약 끝이라고 정했던 데이터가 0x0001 이라는 수였고 받은 데이터중에 운나쁘게 이 수에 곂치는 값이 들어오면 
// 원하던 대로의 바운더리가 그어지지 않고 오동작을 하게 됩니다.
// 그래서 가장 자주 사용하는 방법은 PacketHeader 라는 것을 패킷앞에 붙여서 보내는것입니다. 

struct PacketHeader
{
	// 대부분 이렇게 size, id를 2바이트씩 둔다고 합니다. 
	uint16 size;
	uint16 id;	// 프로토콜 id (ex. 1 = 로그인, 2 = 이동요청 등)
	// 프로토콜 id에 따라 이 패킷이 하는 일을 구별합니다. 
	// 그러면 하는일이 구별이 되면 각 하는일마다의 size를 정해놓으면 헤더의 size를 같이 
	// 들고 있을 이유가 없지 않는가 할 수 있는데 
	// 역할에 따라 패킷이 가변적일 수 있기때문에 size도 같이 기입해 놓는것입니다. 
	// (ex. 이름(wstring), 유저목록(vector) 등) 
};

// 우리가 사용할 정책
// [size(2)][id(2)][data......][size(2)][id(2)][data......]...
// 이런 형태로 패킷을 받을것입니다. 
// 패킷을 받으면 맨앞의 4바이트의 헤더를 해석해 뒤에 이어질 실제 데이터의 크기와 
// 어떤 용도로 사용될것인지를 먼저 확인할 수 있을것입니다. 

class PacketSession : public Session
{
	// 앞으로 컨텐츠단에서는 Session 대신 이 PacketSession을 상속받게 할것입니다. 
public:
	PacketSession();
	~PacketSession();

	// PacketSessionRef는 Types.h 에 추가합니다.
	PacketSessionRef GetPacketsessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	// OnRecv뒤에 sealed를 붙힘으로써 앞으로는 OnRecv는 사용하지 못하게 막았습니다. 
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed;
	// 대신 OnRecvPacket을 사용하게 하고 abstract 키워드로 반드시 재정의해 사용하도록 합니다. 
	virtual int32		OnRecvPacket(BYTE* buffer, int32 len) abstract;
};