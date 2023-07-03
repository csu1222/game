#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"


/*
--------------------
	Session �����
--------------------
*/

class Service;

/*
�̹� �ð��� RecvBuffer Ŭ������ ����� Session�� �����غ��ϴ�. 
*/

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
	
	// ���� ����� �ϵ� �ڵ��ϱ� ���ٴ� enum���� �����մϴ�. 
	enum
	{
		BUFFER_SIZE = 0x10000, // 64kb
	};

public:
	Session();
	virtual ~Session();

public:
	/* �ܺο��� ��� */

	// �Ź� ���۸� ������ Send�ϴ°� �ƴ϶� SendBuffer ��ü�� ������ Send�ϵ��� ����
	void					Send(SendBufferRef sendBuffer);

	// Connect �Լ� �߰� 
	bool					Connect();
	void					Disconnect(const WCHAR* cause);
	
	shared_ptr<Service>		GetService() { return _service.lock(); }
	void					SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* ���� ���� �Լ� */
	void					SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress				GetAddress() { return _netAddress; }
	SOCKET					GetSocket() { return _socket; }
	
	bool					IsConnected() { return _connected; }
	SessionRef				GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
	/* �������̽� ���� */
	virtual HANDLE			GetHandle() override;
	virtual void			Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� �Լ� */
	bool					RegisterConnect();
	bool					RegisterDisconnect();
	void					RegisterRecv();
	// SendEvent�� ��� ������ ����ϱ� �����ϸ鼭 ���ڷ� �޾��� �ʿ䰡 �����ϴ�.
	void					RegisterSend();

	void					ProcessConnect();
	void					ProcessDisconnect();
	void					ProcessRecv(int32 numOfBytes);
	// SendEvent�� ��� ������ ����ϱ� �����ϸ鼭 ���ڷ� �޾��� �ʿ䰡 �����ϴ�.
	void					ProcessSend(int32 numOfBytes);

	void					HandleError(int32 errorCode);

protected:
	/* ������ �ڵ忡�� �������̵� ����� �Լ� */
	
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

	/* ���� ���� TODO */
	RecvBuffer				_recvBuffer;

	/* �۽� ���� TODO */
	// �̹� RegisterSend�� �ɸ� ���¶� �зȴٸ� �� ť�� 
	Queue<SendBufferRef>	_sendQueue;
	// ���� RegisterSend�� �ɸ� ���������� �˱� ���� �÷���
	Atomic<bool>			_sendRegistered = false;

private:
	/* IocpEvent ���� ���� */

	ConnectEvent			_connectEvent;
	DisconnectEvent			_disconnectEvent;
	RecvEvent				_recvEvent;
	// SendEvent�� ��� ������ ����� �� �ְ� ������ �مf���ϴ�.
	SendEvent				_sendEvent;
};


/*
--------------------------
	PacketSession �����
--------------------------
*/

// PacketSession�� �޴��ʰ� ������ �ֽ��ϴ�.
// �޴� �Լ��� ProcessRecv �ȿ��� ���������ʹ� _recvBuffer�� ���� ������� �Բ� �޾��� 
// �������Դϴ�. �ϴ� ���� �� ���۸� ������������ �������ִ� OnRecv�� �б� ������ ��ġ�� 
// �ּ� ReadPos() �� ���� ������ ũ�⸦ �Բ� �ǳ��ְ� �˴ϴ�. 
// �������� ������ �ϴ� GameSession������ OnRecv�� ���� �����͵��� ó�����ְ� �Ǵµ� 
// ��ȯ ���� �󸶳� ó���ߴ����� ��ȯ�մϴ�. (����� ���������� ��θ� ó���ߴٰ� ġ�� ũ�� ���θ� ��ȯ)
// �׷��� ���߿� ����� MMORPG�� ����� �Ǹ� TCP�� Ư���� ���� ��� ��Ŷ�� �ѹ��� ���۵Ǿ��ٰ� ������ �� �������ϴ�. 
// 100byte �� �޾ƾ� �ϴµ� � ������ 20byte���� �ް� �� ���� �־����ϴ�. 
// �׷��� ������ ���� ������(��Ŷ)�� ������ �Դ����� üũ�� �� �ִ� ������ �ʿ��Ѱ��̰� 
// �׸� ���� �츮���� ���������� ������� �մϴ�. 
// ���� �����ϸ� �������߿��� � Ư�� ���ڰ� ������ ��������� �ٿ������� ���ϸ� ����������? 
// ������ �� �ִµ� �׷��� ����� ������ 
// ���� ���̶�� ���ߴ� �����Ͱ� 0x0001 �̶�� ������ ���� �������߿� ��ڰ� �� ���� ��ġ�� ���� ������ 
// ���ϴ� ����� �ٿ������ �׾����� �ʰ� �������� �ϰ� �˴ϴ�.
// �׷��� ���� ���� ����ϴ� ����� PacketHeader ��� ���� ��Ŷ�տ� �ٿ��� �����°��Դϴ�. 

struct PacketHeader
{
	// ��κ� �̷��� size, id�� 2����Ʈ�� �дٰ� �մϴ�. 
	uint16 size;
	uint16 id;	// �������� id (ex. 1 = �α���, 2 = �̵���û ��)
	// �������� id�� ���� �� ��Ŷ�� �ϴ� ���� �����մϴ�. 
	// �׷��� �ϴ����� ������ �Ǹ� �� �ϴ��ϸ����� size�� ���س����� ����� size�� ���� 
	// ��� ���� ������ ���� �ʴ°� �� �� �ִµ� 
	// ���ҿ� ���� ��Ŷ�� �������� �� �ֱ⶧���� size�� ���� ������ ���°��Դϴ�. 
	// (ex. �̸�(wstring), �������(vector) ��) 
};

// �츮�� ����� ��å
// [size(2)][id(2)][data......][size(2)][id(2)][data......]...
// �̷� ���·� ��Ŷ�� �������Դϴ�. 
// ��Ŷ�� ������ �Ǿ��� 4����Ʈ�� ����� �ؼ��� �ڿ� �̾��� ���� �������� ũ��� 
// � �뵵�� ���ɰ������� ���� Ȯ���� �� �������Դϴ�. 

class PacketSession : public Session
{
	// ������ �������ܿ����� Session ��� �� PacketSession�� ��ӹް� �Ұ��Դϴ�. 
public:
	PacketSession();
	~PacketSession();

	// PacketSessionRef�� Types.h �� �߰��մϴ�.
	PacketSessionRef GetPacketsessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	// OnRecv�ڿ� sealed�� �������ν� �����δ� OnRecv�� ������� ���ϰ� ���ҽ��ϴ�. 
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed;
	// ��� OnRecvPacket�� ����ϰ� �ϰ� abstract Ű����� �ݵ�� �������� ����ϵ��� �մϴ�. 
	virtual int32		OnRecvPacket(BYTE* buffer, int32 len) abstract;
};