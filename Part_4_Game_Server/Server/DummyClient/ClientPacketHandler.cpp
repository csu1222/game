#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
// #include "Session.h" �� CorePch�� �߰� �Ͽ����ϴ�. 
#include "Protocol.pb.h"


/*
---------------------------
	ClientPacketHandler
---------------------------
*/
void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	}
}


void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	// ���⼭�� �Ųٷ� ������ȭ Deserialize�� �մϴ�.
	Protocol::S_TEST pkt;

	// Parse �迭 �Լ��� �Ƹ� ���� ���۸� �д� ��� �����ϴ�. 
	// ���ۿ��� PacketHeader�� ���κ��� pkt�� ����ϴ�. 
	ASSERT_CRASH(pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)));

	// �� �Ľ� �Ǿ����� ���� �����͵��� ����غ��ڽ��ϴ�. 
	cout << pkt.id() << " " << pkt.hp() << " " << pkt.attack() << endl;

	// ���� �������� buffs�� ũ�⸦ ����غ��ڽ��ϴ�. 

	cout << "BUFF_SIZE : " << pkt.buffs_size() << endl;

	// buffs�� ���� ��ȸ ������ �������� ���ڽ��ϴ�. 
	for (auto& buff : pkt.buffs())
	{
		cout << "BUFF_ID : " << buff.buffid() << " BUFF_REMAINTIME : " << buff.remaintime() << endl;
		// victims �����͵� Ȯ���ϰڽ��ϴ�. 
		cout << "VICTIMS_SIZE : " << buff.victims_size() << endl;
		for (auto& victim : buff.victims())
		{
			// victim�� uint64 Ÿ���̴� ���� �ٷ� ����� �� �ֽ��ϴ�. 
			cout << victim << " " << endl;
		}
	}
}