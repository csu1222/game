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

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs, wstring name)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
	
	// 고정 데이터를 넣어주는 부분까지는 똑같습니다. 
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();
	bw << id << hp << attack;

	// 가변 데이터 부터 수정이 들어갑니다. 

	// 설계했던대로 가변데이터의 헤더는 offset과 count를 들고 있을건데 구조체로 다뤄주겠습니다.
	struct ListHeader
	{
		uint16 offset;
		uint16 count;
	};
	// buffs의 헤더를 입력할 자리를 예약해 놓습니다. 
	ListHeader* buffsHeader = bw.Reserve<ListHeader>();
	
	// buffsHeader의 내용은 고정데이터가 다 들어오고 난 후, Close전까지만 채워주면 됩니다. 
	// offset은 가변 데이터가 담길 시작 위치를 넣어주면 되는데 현재 상황은 
	// 고정데이터도 다 들어 왔고 가변데이터 헤더도 자리를 만들었으니 바로 이어서 가변데이터가 올겁니다.
	buffsHeader->offset = bw.WriteSize();
	buffsHeader->count = buffs.size();

	// 이제 가변 데이터의 내용물들을 하나씩 넣어줍니다. 
	for (BuffData& buff : buffs)
	{
		bw << buff.buffId << buff.remainTime;
	}

	// 문자열은 생략 

	header->size = bw.WriteSize();
	header->id = S_TEST;

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
