#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

// 이제 Service를 통해 사용할것이기 때문에 날려줍니다. 
//#include "SocketUtils.h"
//
//#include "Listener.h"

#include "Service.h"
#include "Session.h"


/*
ServerServiceRef 를 동적생성할때 3번째 인자로 MakeShared<Session> 이렇게 ()없이 넘겨줍니다. 
이것은 MakeShared의 반환값을 넘겨주는게 아니라 이렇게 생성하는 함수자체를 인자로 넘겨주고 
이걸 함수 포인터라고 하고 우리 Service 클래스에서는 SessionFactory라고 불렀습니다. 

이런식으로 함수포인터를넘겨주는 이득은 나중에 Session을 상속받은 또다른 클래스를 넘겨줄 수 있다는 것입니다. 
예를들어 
class GameSession : public Session
{
}
이때 MakeShared<GameSession> 을 넘겨줄 수도 있습니다. 
*/

int main()
{	
	// 이제 부터 Listener를 직접 만들어 주지 않고 ServerService를 만들어 줄겁니다. 
	// ListenerRef listener = MakeShared<Listener>();
	// Service 객체들은 여러 인자들을 받아주었었는데 우리가 만들었던 MakeShared 에서는 인자를 받지 않는 기본 생성자 버전만 있었습니다. 
	// 그래서 추가했습니다.
	// 인자들은 순서대로 NetAddress, IocpCore 객체, SessionFactory, MaxSessionCount 입니다.
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		 MakeShared<IocpCore>(),
		MakeShared<Session>,
		100);

	// service를 만들었으면 Start를합니다. 여기서 해주는것은 Listener 객체를 만들고 
	// 그 Listener 의 멤버 함수인 StartAccept를 자기자신을 넘겨주면서 AccpetEx를 호출합니다. 
	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}