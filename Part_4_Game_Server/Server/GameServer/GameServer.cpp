#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Job.h"
#include "Room.h"

int main()
{	
	// Test Job
	{
		// [일감 의뢰 내용] : 1번 유저에게 10만큼 힐을 줘라
		// 행동 : Heal
		// 인자 : 1번 유저, 10이라는 힐량
		HealJob healJob;
		healJob._target = 1;
		healJob._healValue = 10;

		// 나중에
		healJob.Execute();
	}

	
	ClientPacketHandler::Init();

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

	while (true)
	{
		GRoom.FlushJob();
		this_thread::sleep_for(1ms);
	}

	// 여러가지 버전의 문자열
	char sendData[1000] = "가";			// CP949 = KS-X-1001(한글문자 2바이트) + KS-X-1003(로마문자 1바이트)
	char sendData2[1000] = u8"가";		// UTF-8 = Unicode를 사용하되 한글 3바이트 로마 1바이트로 변환함
	WCHAR sendData3[1000] = L"가";		// UTF-16 = Unicode를 사용하되 한글/로마 둘다 2바이트 로 거의 Unicode 그대로 사용


	GThreadManager->Join();
}