#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include <tchar.h>
// protobuf
#include "Protocol.pb.h"


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


	while (true)
	{
		// protobuf 는 그냥 임시객체를 만들어 내용을 채우고 임시객체를 통해 serialization을 때려주면 됩니다.
		
		// 앞에 namespace에 Protocol이 온 이유는 Protocol.proto에서 pakage Protocol 명령어를 적어 줬기 때문
		Protocol::S_TEST pkt;

		// protobuf 객체를 그냥 구조체처럼 생각하고 . 을 쩍어봐서 어떤 기능이 있는지를 알아갑니다. 
		// id, hp, attack은 그냥 고정길이 데이터로 set 계열 함수를 호출했습니다.
		pkt.set_id(1000);
		pkt.set_hp(100);
		pkt.set_attack(10);

		// buffs 는 가변길이 데이터입니다. add 계열 함수로 내용물을 채울수 있을것 같습니다. 
		// add_buffs() 를 찍어 보니 따로 인자를 받지는 않고 반환으로 포인터를 반환합니다. 
		// 아마 반환된 포인터부터 내용물을 채워주면 될것같습니다. 

		{
			Protocol::BuffData* data = pkt.add_buffs();
			// BuffData 에도 고정데이터와 가변데이터가 있었습니다 마찬가지로 값을 채웁니다. 
			data->set_buffid(100);
			data->set_remaintime(1.2f);
			// add_victims는 위에 add_buffs와 다르게 uint64 인자를 받아주고 있습니다. 
			// buffs는 message 즉, struct 같은 타입에 가변데이터이다보니 포인터를 반환하고
			// victims는 uint64 타입이기때문에 곧바로 값을 넣어줄 수 있는것 같습니다. 
			data->add_victims(4000);
		}
		{
			Protocol::BuffData* data = pkt.add_buffs();
			// 두번째 buffs
			data->set_buffid(200);
			data->set_remaintime(2.5f);
			data->add_victims(1000);
			data->add_victims(2000);
		}

		// 이렇게 데이터를 넣어주면 전에 배웠던대로 고정데이터를 넣고 난 뒤에 가변데이터를 밀어넣는것이 됩니다.

		// 또 protobuf가 임시객체를 만들어 데이터를 집어넣는 방식이라고 했지만 
		// 꼭 임시객체로만 사용해야하는것은아닙니다. pkt 같은 Protocol 객체를 Session에서 들고 있도록 해서 
		// 재사용하도록 해도 된다는 말입니다. 

		{
			// 그리고 혹시 데이터를 넣을 때 vector에 담긴 데이터를 보내고 싶은데 매번 add_ 함수를 사용해야 하는가 싶습니다.
			// mutable 계열의 함수가 있습니다.
			auto m = pkt.mutable_buffs();
			// m 이라는 객체에서 Add 계열만 봐도 그 위에 처럼 포인터를 반환하는 것도 있고,
			// 이터레이터 begin, end를 받아서 그 범위안에 것들을 다 넣어주는 Add도 있고 
			// rvalue를 받는 ADD도 있습니다. 
			
			vector<Protocol::BuffData> vecBuffs;
			Protocol::BuffData buf1;
			buf1.set_buffid(101);
			buf1.set_remaintime(1.3f);
			buf1.add_victims(1001);

			vecBuffs.push_back(buf1);
			m->Add(vecBuffs.begin(), vecBuffs.end());

		}
		
		// ServerPacketHandler에서 만든 버퍼를 채우는 함수

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}