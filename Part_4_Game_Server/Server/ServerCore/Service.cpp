#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

/*
------------------
	Service ���Ǻ�
------------------
*/
Service::Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: _type(type), _netAddress(address), _iocpCore(core), _sessionFactory(factory), _maxSessionCount(maxSessionCount)
{
}

Service::~Service()
{
}

bool Service::Start()
{
	return false;
}

void Service::CloseService()
{
	// TODO
}

SessionRef Service::CreateSession()
{
	SessionRef session = _sessionFactory();

	// Session ������ Service�� ��� �ֵ��� �Ͽ����ϴ�.
	session->SetService(shared_from_this());

	if (_iocpCore->Register(session) == false)
		return nullptr;
	return session;
}

void Service::AddSession(SessionRef session)
{
	// _sessions �� ���������忡�� �����Ұ��Դϴ�. 
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);
}

void Service::ReleaseSession(SessionRef session)
{
	WRITE_LOCK;
	
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
}


/*
--------------------------
	ClientService ���Ǻ�
--------------------------
*/
// ������ ���ڸ� ��κ� �״�� �Ѱ��ָ� �˴ϴ�. 
ClientService::ClientService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, core, factory, maxSessionCount) 
{
}

bool ClientService::Start()
{
	//TODO
	return true;
}

/*
--------------------------
	ServerService ���Ǻ�
--------------------------
*/
ServerService::ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	:Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	// ServerService�� �����Ҽ� �ִ��� üũ
	if (CanStart() == false)
		return false;
	
	// ������ �����θ�ŭ listener�� ������ �ֽ��ϴ�. 
	_listener = MakeShared<Listener>(); 
	if (_listener == nullptr)
		return false;

	// �������ʹ� Listener::StartAccept �� ���ڷ� NetAddress�� ���� ���� �ʰ� ServerServicr�� ��������ν���
	// NetAddress�� ����Ұ̴ϴ�. ��, ���⼭�� ServerService �ڱ� �ڽ��� shared_ptr�� ���ڷ� �ݴϴ�.
	// �׷��� ���ؼ� shared_from_this�� ȣ���Ϸ��� �ϴµ� �θ� Ŭ������ Service�� shrard_ptr�� ��ȯ�̵ȴٰ� �մϴ�. 
	// �׷��� ĳ������ ���� ServerService�� �ٲ��ݴϴ�. 
	ServerServiceRef service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;

	return true;
}

void ServerService::CloseService()
{
	// TODO
	Service::CloseService();
}
