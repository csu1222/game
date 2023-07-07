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


	// 여러가지 버전의 문자열
	char sendData[1000] = "가";			// CP949 = KS-X-1001(한글문자 2바이트) + KS-X-1003(로마문자 1바이트)
	char sendData2[1000] = u8"가";		// UTF-8 = Unicode를 사용하되 한글 3바이트 로마 1바이트로 변환함
	WCHAR sendData3[1000] = L"가";		// UTF-16 = Unicode를 사용하되 한글/로마 둘다 2바이트 로 거의 Unicode 그대로 사용
	TCHAR sendData4[1000] = _T("가");

	while (true)
	{
		// 가변데이터를 만듭니다.
		vector<BuffData> buffs{ BuffData{100, 1.5f}, BuffData{200, 2.3f} , BuffData{300, 0.7f} };

		// 패킷 핸들러에서 패킷을 만들어주는 기능을 추가 했습니다. 
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_TEST(1001, 100, 10, buffs, L"안녕하세요");

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}