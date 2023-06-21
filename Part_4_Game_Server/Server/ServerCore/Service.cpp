#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

/*
------------------
	Service 정의부
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

	// Session 에서도 Service를 들고 있도록 하였습니다.
	session->SetService(shared_from_this());

	if (_iocpCore->Register(session) == false)
		return nullptr;
	return session;
}

void Service::AddSession(SessionRef session)
{
	// _sessions 를 여러스레드에서 접근할것입니다. 
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
	ClientService 정의부
--------------------------
*/
// 생성자 인자를 대부분 그대로 넘겨주면 됩니다. 
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
	ServerService 정의부
--------------------------
*/
ServerService::ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	:Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	// ServerService가 시작할수 있는지 체크
	if (CanStart() == false)
		return false;
	
	// 서버쪽 서비스인만큼 listener를 가지고 있습니다. 
	_listener = MakeShared<Listener>(); 
	if (_listener == nullptr)
		return false;

	// 이제부터는 Listener::StartAccept 의 인자로 NetAddress를 직접 받지 않고 ServerServicr의 멤버변수로써의
	// NetAddress를 사용할겁니다. 즉, 여기서는 ServerService 자기 자신의 shared_ptr을 인자로 줍니다.
	// 그러기 위해서 shared_from_this를 호출하려고 하는데 부모 클래스인 Service의 shrard_ptr로 반환이된다고 합니다. 
	// 그래서 캐스팅을 통해 ServerService로 바꿔줍니다. 
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
