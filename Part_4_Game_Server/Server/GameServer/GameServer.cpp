#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"

/*
GameServer 메인 스레드에서 직접 접속한 모든 클라이언트에 Send를 뿌립니다. 
*/

int main()
{	
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		 MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

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

	// 메인 스레드에서 데이터를 뿌립니다. 
	// 이렇게 게임서버쪽에서 로직을 계산해 전체 클라이언트에 뿌리는 상황이 빈번할겁니다.
	// 예를들어 몬스터가 리젠되었다는 게임 서버에서 자체 처리하는 로직을 모든 클라이언트에 적용시킬때 같은

	// 이 문자열이 임시로 모든 클라에 뿌릴 데이터입니다. 
	char sendData[1000] = "Hello World!";

	while (true)
	{
		// 메인스레드의 TLS에 SendBuffer를 할당받습니다.
		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

		BYTE* buffer = sendBuffer->Buffer();
		(reinterpret_cast<PacketHeader*>(buffer)->size) = (sizeof(sendData) + sizeof(PacketHeader));
		(reinterpret_cast<PacketHeader*>(buffer)->id) = 1;	// 1번 id가 Hello World! 라고 정해둡시다.

		// 이제 버퍼에 데이터를 넣어줍니다. 
		::memcpy(&buffer[4], sendData, sizeof(sendData));
		sendBuffer->Close((sizeof(sendData) + sizeof(PacketHeader)));
		
		// 데이터를 받았으면 자신 연결되 있는 클라뿐만 아니라 세션 매니저의 모든 세션에 Send
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}