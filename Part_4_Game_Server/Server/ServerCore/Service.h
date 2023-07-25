#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

// �ϴ� ���� Ÿ���� ����, Ŭ�� �ΰ����� �ֽ��ϴ�.
enum class ServiceType : uint8
{
	Server,
	Client,
};

/*
------------------
	Service ����� 
------------------
*/

/*
std::function Ŭ������ �Լ� ������, �Լ� ��ü�� ����ϴ�.
<> �ȿ� ������ Ÿ���� ��ȯ�ϰ� () �ȿ��� � ���ڸ� �޾��ִ����� ���մϴ�.
���� �ڵ��� �ǹ̴� ������ 
SessionFactory() �� ȣ���ϸ� SessionRef ��ü�� �������ݴϴ�.
*/
using SessionFactory = function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	// ���ڸ���� �����ϴ�.
	// 1) �� ���񽺰� � Ÿ���� ��������
	// 2) �ּ� ����ü�ε� ������ �ڽ��� �ּ�, Ŭ���̾�Ʈ�� ���� �����Ϸ��� ������ �ּ�
	// 3) IocpCore�� Shared_ptr, IocpCore�� �� �ϳ��� ��������� �������̱� ������ � �ھ ��������� �޽��ϴ�. 
	// 4) SeesionFactory ��� ������ ������ִ� �Լ��� �޽��ϴ�. 
	// 5) ���������� �ִ� ������ ��ΰ�? �ִ� �������� �����ݴϴ�.
	Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool	Start() abstract;
	// CanStart�� �������� �ϴ� _sessionFactory�� �ִ����� üũ�ϰڽ��ϴ�. 
	bool			CanStart() { return _sessionFactory != nullptr; }

	virtual void	CloseService();
	// SetSessionFactory �� �����ڿ��� �־���� SessionFactory�� ���� �ȵ�� ���� �������ݴϴ�.
	void			SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	// �����ϴ� ��� ���ǿ��� ���۸� ����
	void			Broadcast(SendBufferRef sendBuffer);
	// ���� ������ ����� �ܰ� ���ÿ� IocpCore�� ������ݴϴ�.
	SessionRef		CreateSession();
	// �̹� ������� ������ �߰��ϴ� �Լ�
	void			AddSession(SessionRef session); 
	// � ������ ���̻� ������� �ʴ´ٰ��Ҷ� �����ִ� �Լ�
	void			ReleaseSession(SessionRef session);
	int32			GetCurrentSessionCount() { return _sessionCount; }
	int32			GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType		GetServiceType() { return _type; }
	NetAddress		GetNetAddress() { return _netAddress; }
	// GetIocpCore�� ��ȯŸ���� ���������� �ߴµ� ���� IocpCore ��ü�� ���� ī��Ʈ�� �ø��� �ʱ� ����
	// IocpCore�� �ܺο��� ������ ������ �����ϰ� �ɰ��ε� �׷��� ���� ������� �ʾƵ� ���� ī��Ʈ�� �޸𸮸� �����ϰ� �ֽ��ϴ�. 
	IocpCoreRef&	GetIocpCore() { return _iocpCore; }

protected:
	// ��� ������ 
	USE_LOCK;	// ���� �����忡�� sessions�� �����Ұ��̴� ���� ����Ұ��Դϴ�.

	// �����ڿ��� �޴� ���ڵ��� ������ ������ 
	ServiceType		_type;
	NetAddress		_netAddress = {};
	IocpCoreRef		_iocpCore;

	Set<SessionRef> _sessions;
	int32			_sessionCount = 0;	// �׽�Ʈ ����Ҷ� ����� ���� ����ϴ� ������ ��
	int32			_maxSessionCount = 0;	// �ִ� ������ 
	SessionFactory	_sessionFactory;
};


/*
--------------------------
	ClientService �����
--------------------------
*/

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}

	virtual bool Start() override;
};

/*
--------------------------
	ServerService �����
--------------------------
*/

class ServerService : public Service
{
public:
	ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}

	virtual bool Start() override;
	virtual void CloseService() override;
private:
	ListenerRef _listener = nullptr;
};