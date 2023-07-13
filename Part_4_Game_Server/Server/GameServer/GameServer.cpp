#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
// TCHAR 사용할때 필요
#include <tchar.h>


int main()
{	
	// -----------------------
	
	PKT_S_TEST pkt;
	pkt.hp = 1;
	pkt.id = 2;
	pkt.attack = 3;

	// -----------------------
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


	// 여러가지 버전의 문자열
	char sendData[1000] = "가";			// CP949 = KS-X-1001(한글문자 2바이트) + KS-X-1003(로마문자 1바이트)
	char sendData2[1000] = u8"가";		// UTF-8 = Unicode를 사용하되 한글 3바이트 로마 1바이트로 변환함
	WCHAR sendData3[1000] = L"가";		// UTF-16 = Unicode를 사용하되 한글/로마 둘다 2바이트 로 거의 Unicode 그대로 사용
	TCHAR sendData4[1000] = _T("가");

	while (true)
	{
		// PKT_S_TEST_WRITE 객체를 만들어서 버퍼를 채웁니다. 
		PKT_S_TEST_WRITE pktWriter(1001, 100, 10);
		
		PKT_S_TEST_WRITE::BuffsList buffsList = pktWriter.ReserveBuffsList(3);

		buffsList[0] = { 100, 1.5f };
		buffsList[1] = { 200, 2.3f };
		buffsList[2] = { 300, 0.7f };

		// BuffsList를 하나씩 순회하면서 그 뒤에 victims의 가변길이 데이터를 만들어 줍니다.
		PKT_S_TEST_WRITE::BuffsVictimsList vic0 = pktWriter.ReserveBuffsVictimsList(&buffsList[0], 3);
		{
			vic0[0] = 1000;
			vic0[1] = 2000;
			vic0[2] = 3000;
		}
		PKT_S_TEST_WRITE::BuffsVictimsList vic1 = pktWriter.ReserveBuffsVictimsList(&buffsList[1], 1);
		{
			vic1[0] = 1000;
		}
		PKT_S_TEST_WRITE::BuffsVictimsList vic2 = pktWriter.ReserveBuffsVictimsList(&buffsList[2], 2);
		{
			vic2[0] = 1000;
			vic2[1] = 2000;
		}
		// 이런 식으로 리스트 안에 리스트를 만듭니다. 
		// 물론 좀 더 편하게 관리하고 싶으면 계층적으로 관리해서 PKT_S_TEST_WRITE 안에서 다 관리하는게 아니라
		// BuffsListItem 안에 Reserve 함수 같은걸 따로 만들어 주는게 좋을것입니다. 

		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}