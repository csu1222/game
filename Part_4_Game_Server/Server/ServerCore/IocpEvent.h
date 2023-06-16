#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	//PreRecv, // 0Byte Recv라는 고급 기술에서 사용되는 타입
	Recv,
	Send,
};

/*
----------------
	IocpEvent
----------------
*/

/*
Overlapped 구조체를 대신할 것이기 때문에 가장 앞에 Overlapped 구조체가 있거나
상속을 받아줘야합니다. 여기서는 상속하는 방식으로 하겠습니다.
-주의할점-
상위 클래스가 될것이라고 virtual 키워드를 쓰면 offset 0번에 OVERLAPPED 가 아닌 다른 정보가 
들어가게되니 사용하면 안된다
*/
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void		Init();
	EventType	GetType() { return _type; }
protected:
	EventType	_type;
};


/*
----------------
	ConnectEvent
----------------
*/
// 아주 간단하게 EventType을 Connect로 들고 있게 합니다. 
class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};

/*
----------------
	AcceptEvent
----------------
*/
// Accept는 예외적으로 EventType 외에 추가적인 변수가 있습니다. 
// 그냥 accept 함수를 호출하면 반환으로 clientSocket이 반환 되었는데 앞으로 사용하려는 
// AcceptEx의 인자를 보면 listenSocket 과 clientSocket으로 사용할 소켓도 같이 받아주고 있습니다.
// 다른 IO 함수들과 달리 AcceptEvent 에서는 추가적으로 필요한 인자를 들고 있어야 합니다. 
//
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

	void		SetSession(Session* session) { _session = session; }
	Session*	GetSession() { return _session; }
private:
	// Session을 연동받아서 나중에 이 이벤트가 어떤 세션에 대한 이벤트인지를 기억합니다.
	Session*	_session = nullptr;
};

/*
----------------
	RecvEvent
----------------
*/
class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) { }
};

/*
----------------
	SendEvent
----------------
*/
class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) { }
};
