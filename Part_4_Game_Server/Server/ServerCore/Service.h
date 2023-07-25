#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

// 일단 서비스 타입은 서버, 클라 두가지가 있습니다.
enum class ServiceType : uint8
{
	Server,
	Client,
};

/*
------------------
	Service 선언부 
------------------
*/

/*
std::function 클래스는 함수 포인터, 함수 객체를 만듭니다.
<> 안에 들어오는 타입을 반환하고 () 안에는 어떤 인자를 받아주는지를 뜻합니다.
밑의 코드의 의미는 앞으로 
SessionFactory() 을 호출하면 SessionRef 객체를 생성해줍니다.
*/
using SessionFactory = function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	// 인자목록이 많습니다.
	// 1) 이 서비스가 어떤 타입의 서비스인지
	// 2) 주소 구조체인데 서버는 자신의 주소, 클라이언트의 경우는 접속하려는 서버의 주소
	// 3) IocpCore의 Shared_ptr, IocpCore를 꼭 하나만 사용하지는 않을것이기 때문에 어떤 코어를 사용할지를 받습니다. 
	// 4) SeesionFactory 라고 세션을 만들어주는 함수를 받습니다. 
	// 5) 마지막으로 최대 세션은 몇개인가? 최대 동접수를 정해줍니다.
	Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool	Start() abstract;
	// CanStart의 조건으로 일단 _sessionFactory가 있는지만 체크하겠습니다. 
	bool			CanStart() { return _sessionFactory != nullptr; }

	virtual void	CloseService();
	// SetSessionFactory 는 생성자에서 넣어줬던 SessionFactory가 맘에 안들면 새로 설정해줍니다.
	void			SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	// 관리하는 모든 세션에게 버퍼를 전송
	void			Broadcast(SendBufferRef sendBuffer);
	// 새로 세션을 만들어 줌과 동시에 IocpCore에 등록해줍니다.
	SessionRef		CreateSession();
	// 이미 만들어진 세션을 추가하는 함수
	void			AddSession(SessionRef session); 
	// 어떤 세션을 더이상 사용하지 않는다고할때 꺼내주는 함수
	void			ReleaseSession(SessionRef session);
	int32			GetCurrentSessionCount() { return _sessionCount; }
	int32			GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType		GetServiceType() { return _type; }
	NetAddress		GetNetAddress() { return _netAddress; }
	// GetIocpCore만 반환타입을 참조값으로 했는데 괜히 IocpCore 객체의 참조 카운트를 늘리지 않기 위함
	// IocpCore를 외부에서 꺼내는 동작을 자주하게 될것인데 그러면 딱히 사용하지 않아도 참조 카운트가 메모리를 점유하고 있습니다. 
	IocpCoreRef&	GetIocpCore() { return _iocpCore; }

protected:
	// 멤버 변수들 
	USE_LOCK;	// 여러 스레드에서 sessions에 접근할것이니 락을 사용할것입니다.

	// 생성자에서 받는 인자들을 저장할 변수들 
	ServiceType		_type;
	NetAddress		_netAddress = {};
	IocpCoreRef		_iocpCore;

	Set<SessionRef> _sessions;
	int32			_sessionCount = 0;	// 테스트 출력할때 사용할 지금 사용하는 세션의 수
	int32			_maxSessionCount = 0;	// 최대 동접수 
	SessionFactory	_sessionFactory;
};


/*
--------------------------
	ClientService 선언부
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
	ServerService 선언부
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