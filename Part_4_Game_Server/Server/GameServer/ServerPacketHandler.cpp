#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

/*
--------------------------
	ServerPacketHandler
--------------------------
*/
void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	// ClientPacketHandler 에서는 >> 연산자로 그냥 꺼냈었는데 
	// 사실 header의 id만 살펴보고 판별하면 되니까 Peek() 을 사용해도 됩니다.
	br.Peek(&header);

	// 현재는 클라이언트에서 패킷을 받도록 설계한적이 없기 때문에 딱히 해줄게 없습니다. 
	switch (header.id)
	{
	default:
		break;
	}
}

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();
	// 인자로 데이터를 밀어넣습니다.
	bw << id << hp << attack;

	// 가변 데이터 
	// 먼저 가변데이터의 사이즈를 밀어 넣습니다. 메모리가 아까우니 uint16으로 캐스팅합니다.
	bw << (uint16)buffs.size();

	// 이제 가변 데이터의 내용물들을 하나씩 넣어줍니다. 
	for (BuffData& buff : buffs)
	{
		bw << buff.buffId << buff.remainTime;
	}

	header->size = bw.WriteSize();
	header->id = S_TEST;

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
