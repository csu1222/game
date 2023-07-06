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
	// ClientPacketHandler ������ >> �����ڷ� �׳� ���¾��µ� 
	// ��� header�� id�� ���캸�� �Ǻ��ϸ� �Ǵϱ� Peek() �� ����ص� �˴ϴ�.
	br.Peek(&header);

	// ����� Ŭ���̾�Ʈ���� ��Ŷ�� �޵��� ���������� ���� ������ ���� ���ٰ� �����ϴ�. 
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
	// ���ڷ� �����͸� �о�ֽ��ϴ�.
	bw << id << hp << attack;

	// ���� ������ 
	// ���� ������������ ����� �о� �ֽ��ϴ�. �޸𸮰� �Ʊ��� uint16���� ĳ�����մϴ�.
	bw << (uint16)buffs.size();

	// ���� ���� �������� ���빰���� �ϳ��� �־��ݴϴ�. 
	for (BuffData& buff : buffs)
	{
		bw << buff.buffId << buff.remainTime;
	}

	header->size = bw.WriteSize();
	header->id = S_TEST;

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
