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

		// ���� ����Ʈ 
		uint16 victimsOffset;
		uint16 victimsCount;

		// BuffsListItem ���ο����� ������ �񱳸� �ϰ� �߿Դ����� bool�� ��ȯ�ϰ� ������ size���ٰ�
		// Ȯ���� ������ ũ�⸦ �����̴ٰϴ�. 
		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			// victims ���� ���� ������ ����� packetSize���� ũ�ٸ� ������ �ֽ��ϴ�.
			if (victimsOffset + victimsCount * sizeof(uint64) > packetSize)
				return false;

			// ����ߴٸ� ��Ŷ�� ����� �����ݴϴ�.
			size += victimsCount * sizeof(uint64);
			return true;			
		}
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

		// Buffs ���������� ũ�� �߰� 
		if (buffOffset + buffCount * sizeof(BuffsListItem) > packetSize)
			return false;
		
		size += buffCount * sizeof(BuffsListItem);

		// BuffsList�� ��ȸ�ϸ鼭 victims �������� ũ�⸦ Ȯ���մϴ�. 
		BuffsList buffsList = GetBuffsList();
		for (int32 i = 0; i < buffsList.Count(); i++)
		{
			if (buffsList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;
		}

		// ���� ũ�� ��
		if (size != packetSize)
			return false;


		return true;
	}

	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
	using BuffsVictimsList = PacketList<uint64>;

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

	BuffsVictimsList GetBuffsVictimsList(BuffsListItem* buffsItem)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsItem->victimsOffset;

		return BuffsVictimsList(reinterpret_cast<uint64*>(data), buffsItem->victimsCount);
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

	for (auto& buff : buffs)
	{
		cout << "Buff Info : " << buff.buffId << " " << buff.remainTime << endl;

		PKT_S_TEST::BuffsVictimsList victims =  pkt->GetBuffsVictimsList(&buff);
		cout << "Victims Count : " << victims.Count() << endl;
		for (auto& victim : victims)
		{
			cout << "Victim : " << victim << endl;
		}
	}
}
