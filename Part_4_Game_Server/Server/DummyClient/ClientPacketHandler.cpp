#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
// #include "Session.h" �� CorePch�� �߰� �Ͽ����ϴ�. 


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

#pragma pack(1)
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;
	};

	uint16 packetSize;
	uint16 packetId;
	uint64 id;
	uint32 hp;
	uint16 attack;
	// �������� �����͸� �����ϴ� ������ 
	uint16 buffOffset; // �������� �����Ͱ� �����ϴ� �ε���
	uint16 buffCount;
	
	// ��Ŷ�� ������ ���������� ó���ǰ� �ִ����� Ȯ���ϴ� �Լ� 
	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_TEST);
		// �� üũ�� ��� ���������͵��� ���� �ִ����� Ȯ��
		if (packetSize < size)
			return false;
		
		size += buffCount * sizeof(BuffsListItem);

		if (size != packetSize)
			return false;

		if (buffOffset + buffCount * sizeof(BuffsListItem) > packetSize)
			return false;

		return true;
	}

	// BuffsList�� BuffsListItem�� Ÿ������ �� PacketList ��� ���� �մϴ�. 
	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;

	// BuffsList�� ������ �Լ�
	BuffsList GetBuffsList()
	{
		// PKT_S_TEST�� �����ּҿ� buffOffset ��ŭ�� ���ϸ� ������������ ���� �ּ��Դϴ�. 
		// BYTE* �� 1����Ʈ¥���ϱ� uint16�� buffOffset�� ���ϸ� �ּҰ� ���ɴϴ�.
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffOffset;

		// ��ȯ�Ҷ��� �ٽ� PKT_S_TEST::BuffsListItem*�� ĳ�����ؼ� PakcetList�� �����ڿ� �Ѱ��ݴϴ�.
		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffsListItem*>(data), buffCount);
	}
};
#pragma pack()

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	// ������ �����͸� ���� �ӽ� ��ü�� �������� �ʾƵ� ���� ������?

	// BYTE �迭�� buffer�� �ٷ� ĳ�����ؼ� PKT_S_TEST�� �ؼ��� �մϴ�.
	PKT_S_TEST* pkt = reinterpret_cast<PKT_S_TEST*>(buffer);

	// PacketSession::OnRecv�� �������鼭 �ϴ� PacketHeader ��ŭ�� ��� �ִٰ� ������ �� �ֽ��ϴ�. 
	// �� ����� ���� ������� ����ũ�⸦ ���ϱ�� �ߴµ� �װ� �����Ǿ� �ִ����� Ȯ���غ����մϴ�. 

	// Validate ���� ������� ���������ͱ����� ���Դ��� ���Դٸ� �� ������� ������ ���������͵� ���� ����� ���Դ����� 
	// Ȯ���մϴ�. 
	if (pkt->Validate() == false)
		return;

	// ������� �Դٸ� �ϴ� pkt ��� �����Ͱ� buffer�� �ִ� �����͵��� PKT_S_TEST�� �ؼ��մϴ�.

	// �׷��ٸ� ����ó�� ���������͵鵵 ���� ���� ��ü�� ����� �޾��� �ʿ���� 
	// �����͵��� �ּҸ� �˼� �ֱ⶧���� ��ٷ� ���� �� �� �����̴ϴ�. 
	// �����Ϳ� �����ϱ� ���ϰ� ���� Ŭ������ ��������ϴ�.

	// ���������͸� PacketList�� ������ �ݴϴ�. 

	PKT_S_TEST::BuffsList buffs = pkt->GetBuffsList();

	// buffs�� buffer�� �����͸� �����Ѱ� �ƴ� �׳� �����ͷ� ����Ű�� �ִ°��Դϴ�. 

	// ���� ���� �����ϴ°� �ƴ� PacketList�� ������ [] �����ڷ� ���� �����Ҽ� �ְ� �˴ϴ�. 
	cout << "BuffCount : " << buffs.Count() << endl;
	for (int32 i = 0; i < buffs.Count(); i++)
	{
		cout << "Buff Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}

	for (auto it = buffs.begin(); it != buffs.end(); ++it)
		cout << "Buff Info : " << it->buffId << " " << it->remainTime << endl;

	for (auto& buff : buffs)
		cout << "Buff Info : " << buff.buffId << " " << buff.remainTime << endl;
}
