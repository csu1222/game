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

	// 가변 데이터 2탄 wstring 문자열 
	// 위쪽 가변데이터와 똑같은 방식을 사용합니다.
	// 먼저 문자열의 크기를 밀어 넣습니다. 
	// UTF-16 기준으로 size()를 사용하면 마지막에 null종단문자는 사이즈에 포함되지 않습니다. 
	// 어짜피 UTF-16을 사용한다고 알고 있으면 받는쪽에서 처리해줘도 됩니다. 
	bw << (uint16)name.size();

	// BufferWrite.Write 함수는 void* 타입으로 src를 받아주고 있엇습니다. 
	// 사이즈는 문자열의 갯수에 문자 한개당 크기(sizeof(WCHAR))를 곱해주었습니다. 
	bw.Write((void*)name.data(), name.size() * sizeof(WCHAR));

	header->size = bw.WriteSize();
	header->id = S_TEST;

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
