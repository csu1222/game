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

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs, wstring name)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
	
	// ���� �����͸� �־��ִ� �κб����� �Ȱ����ϴ�. 
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();
	bw << id << hp << attack;

	// ���� ������ ���� ������ ���ϴ�. 

	// �����ߴ���� ������������ ����� offset�� count�� ��� �����ǵ� ����ü�� �ٷ��ְڽ��ϴ�.
	struct ListHeader
	{
		uint16 offset;
		uint16 count;
	};
	// buffs�� ����� �Է��� �ڸ��� ������ �����ϴ�. 
	ListHeader* buffsHeader = bw.Reserve<ListHeader>();
	
	// buffsHeader�� ������ ���������Ͱ� �� ������ �� ��, Close�������� ä���ָ� �˴ϴ�. 
	// offset�� ���� �����Ͱ� ��� ���� ��ġ�� �־��ָ� �Ǵµ� ���� ��Ȳ�� 
	// ���������͵� �� ��� �԰� ���������� ����� �ڸ��� ��������� �ٷ� �̾ ���������Ͱ� �ð̴ϴ�.
	buffsHeader->offset = bw.WriteSize();
	buffsHeader->count = buffs.size();

	// ���� ���� �������� ���빰���� �ϳ��� �־��ݴϴ�. 
	for (BuffData& buff : buffs)
	{
		bw << buff.buffId << buff.remainTime;
	}

	// ���ڿ��� ���� 

	header->size = bw.WriteSize();
	header->id = S_TEST;

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
