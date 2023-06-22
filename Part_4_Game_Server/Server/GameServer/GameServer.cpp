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
Session 클래스의 멤버 함수중 Process류 함수에서 호출하던 오버로딩용 함수들을 에코서버처럼 동작하게
만들어 봤습니다. 
*/
class GameSession : public Session
{
public:
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		// Echo Server 처럼 동작하게 만들어 줘 보겠습니다.
		cout << "OnRecv Len = " << len << endl;
		Send(buffer, len);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

int main()
{	
	// Session 대신 GameSession을 사용했습니다. 
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		 MakeShared<IocpCore>(),
		MakeShared<GameSession>,
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