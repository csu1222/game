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
// protobuf
#include "Protocol.pb.h"


int main()
{	
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


	// 여러가지 버전의 문자열
	char sendData[1000] = "가";			// CP949 = KS-X-1001(한글문자 2바이트) + KS-X-1003(로마문자 1바이트)
	char sendData2[1000] = u8"가";		// UTF-8 = Unicode를 사용하되 한글 3바이트 로마 1바이트로 변환함
	WCHAR sendData3[1000] = L"가";		// UTF-16 = Unicode를 사용하되 한글/로마 둘다 2바이트 로 거의 Unicode 그대로 사용


	GThreadManager->Join();
}