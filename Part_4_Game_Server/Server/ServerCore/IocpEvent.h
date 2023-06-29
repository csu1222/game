#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
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

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void		Init();

public:
	EventType	eventType;
	IocpObjectRef owner;
};


/*
----------------
	ConnectEvent
----------------
*/
class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};

/*
----------------
	DisconnectEvent
----------------
*/
class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) { }
};


/*
----------------
	AcceptEvent
----------------
*/
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

	
public:
	SessionRef	session = nullptr;
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

	// 이전에는 BYTE 타입으로만 버퍼를 받고 있었는데 이제 SendBuffer 단위로 
	// 받아 관리하도록 하였습니다. 
	Vector<SendBufferRef> sendBuffers;
};
