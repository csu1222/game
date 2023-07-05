#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"

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

	char sendData[1000] = "Hello World!";

	while (true)
	{
		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

		// BufferWriter 객체로 sendBuffer의 영역을 찝어 줍니다. 
		BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());

		// PacketHeader가 들어갈 자리를 Reserve 합니다. 반환 값으로 바로 헤더의 시작주소를 반환 합니다.
		PacketHeader* header = bw.Reserve<PacketHeader>();

		// 이번에는 넣어줄 데이터를 좀 다르게 해보겠습니다.
		// id(uint64), 체력(uint32), 공격력(uint16) 라고 해보겠습니다. 
		bw << (uint64)1001 << (uint32)100 << (uint16)10;
		// << 연산자를 사용해 캐스팅한 데이터를 넣어준 다음은 그 타입 만큼 _pos를 옯기니까 
		// 연속해서 데이터를 넣을 수 있습니다. 그리고 << 연산자의 반환이 
		// 데이터를 넣고 난 다음의 자신의 포인터다 보니 연속해서 사용할 수 있습니다.

		// << 연산자와 비슷하게 Write() 함수도 사용할 수 있는데 다른점은 
		// << 연산자는 버퍼의 남은 공간인 FreeSize()를 따로 체크하지 않고
		// Write()함수는 남은 공간을 체크한다는 점입니다. 
		// 꼭 <<연산자라고 해서 공간체크를 하면 안된다는것도 아니니 본인 구현하기 나름입니다.
		bw.Write(sendData, sizeof(sendData));
		
		// buffer를 굳이 캐스팅 하지 않아도 bw.Reserve로 받아놓은 header에 값을 넣어줍니다.
		// 그리고 미리 자리를 예약해 뒀기 때문에 값을 넣어주는 시점이 더 자유로워집니다. 
		header->size = bw.WriteSize();
		header->id = 1;
		
		// size를 계산하는걸 이제는 직접 하지 않아도 됩니다. 
		sendBuffer->Close(bw.WriteSize());
		
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}