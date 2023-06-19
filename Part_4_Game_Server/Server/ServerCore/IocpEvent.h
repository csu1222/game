#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	//PreRecv, // 0Byte Recv��� ��� ������� ���Ǵ� Ÿ��
	Recv,
	Send,
};

/*
----------------
	IocpEvent
----------------
*/

/*
Overlapped ����ü�� ����� ���̱� ������ ���� �տ� Overlapped ����ü�� �ְų�
����� �޾�����մϴ�. ���⼭�� ����ϴ� ������� �ϰڽ��ϴ�.
-��������-
���� Ŭ������ �ɰ��̶�� virtual Ű���带 ���� offset 0���� OVERLAPPED �� �ƴ� �ٸ� ������ 
���ԵǴ� ����ϸ� �ȵȴ�
*/
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void		Init();

	// private���� public���� ������µ� ��� ������ ���������� �����̴� ���� �������ϴ�.
public:
	EventType	eventType;
	// �̺�Ʈ�� ���� ��ü�� shared_ptr�� ��� �ֽ��ϴ�. 
	IocpObjectRef owner;
};


/*
----------------
	ConnectEvent
----------------
*/
// ���� �����ϰ� EventType�� Connect�� ��� �ְ� �մϴ�. 
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
// Accept�� ���������� EventType �ܿ� �߰����� ������ �ֽ��ϴ�. 
// �׳� accept �Լ��� ȣ���ϸ� ��ȯ���� clientSocket�� ��ȯ �Ǿ��µ� ������ ����Ϸ��� 
// AcceptEx�� ���ڸ� ���� listenSocket �� clientSocket���� ����� ���ϵ� ���� �޾��ְ� �ֽ��ϴ�.
// �ٸ� IO �Լ���� �޸� AcceptEvent ������ �߰������� �ʿ��� ���ڸ� ��� �־�� �մϴ�. 
//
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

	
	// ��� ������ �׳� public���� ��� ���� �ǵ帮�ڽ��ϴ�. 
public:
	// Session�� �����޾Ƽ� ���߿� �� �̺�Ʈ�� � ���ǿ� ���� �̺�Ʈ������ ����մϴ�.
	// shared_ptr�� ��� �ְ� �ؼ� ���� ī������ ���ݴϴ�.
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
};
