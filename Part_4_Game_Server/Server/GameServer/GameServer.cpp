#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "SocketUtils.h"

#include "Listener.h"

// IocpCore를 통해 비동기 IO함수인 AcceptEx를 테스트 해보겠습니다. 
int main()
{
	// 나중에는 지금처럼 직접 만들지는 않겠지만 테스트를 위해 Listener를 만들어 주겠습니다.
	Listener listener;
	
	// StartAccept 함수내부에서 알아서 리슨소켓생성, CP에 등록, 각종옵션세팅,bind, listen, RegisterAccept
	// 까지 해줄겁니다.
	listener.StartAccept(NetAddress(L"127.0.0.1", 7777));

	// StartAccept 내부에서 RegisterAccept를 호출한후 누군가 접속을 요청했다고 하면 
	// IocpCore::Dispatch 를 통해 누가 접속했는지를 인지할 수 있게 됩니다. 
	// 그러면 Dispatch는 어디서 관찰하고 있을것이냐면 다른 스레드를 만들어서 관찰시킬겁니다.

	// 멀티스레드를 만들겁니다. 보통 멀티스레드의 갯수는 CPU 코어 갯수 ~ 코어 갯수 1.5 배 정도가 적당하다고 합니다.
	// 괜히 많아봤자 컨택스트 스위칭 비용만 늘어납니다. 
	for (int32 i = 0; i < 5; i++)
	{
		// 스레드를 만듭니다. 하는일은 무한루프를 돌면서 계속 전역 IocpCore 객체를 Dispatch하는겁니다.
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					GIocpCore.Dispatch();
				}
			});
	}

	GThreadManager->Join();
}