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

	// ��Ŷ id�� ���� ó���� ���ݴϴ�. 
	// ��쿡 ���� ��¥ ��Ŷ������ ���� ���鰳�� switch ���� ����ϱ⵵ �Ѵ�ϴ�.

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	}
}

#pragma pack(1)
// ��Ŷ ���� TEMP
struct PKT_S_TEST
{
	// �������� �������� ������ ��꿡 �ʿ��մϴ�.
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
	//vector<BuffData> buffs;
	//wstring name;
	
	// ��Ŷ�� ������ ���������� ó���ǰ� �ִ����� Ȯ���ϴ� �Լ� 
	bool Validate()
	{
		// ���� �׽�Ʈ�� �ϸ�˴ϴ�. 
		uint32 size = 0;

		// ���� PKT_S_TEST�� ����� ���մϴ�.
		size += sizeof(PKT_S_TEST);

		// �������� �������� ����� ���մϴ�. 
		size += buffCount * sizeof(BuffsListItem);

		// �̷��� ���� ����� ����� packetSize�� �ٸ��ٰ� �ϸ� ������ �ֽ��ϴ�.
		if (size != packetSize)
			return false;

		// �������� �������� ���� �ε����� buffOffset�� ���� ���� ������ �ٽ� üũ
		if (buffOffset + buffCount * sizeof(BuffsListItem) > packetSize)
			return false;

		return true;

	}
};
#pragma pack()

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	// ��Ŷ�� ó���� �ϴ� �Լ��̱������� header ���� ������� ������ �½��ϴ�. 
	BufferReader br(buffer, len);

	// ��Ŷ�ȿ� header�� �ش��ϴ� ������ �־����ϴ�.
	//PacketHeader header;
	//br >> header;

	// ���� ������ũ�Ⱑ ��� ���������ʹ� ��� �ִ��� üũ�մϴ�. 
	if (len < sizeof(PKT_S_TEST))
		return;

	// ��Ŷ���� ����ü ����� �����͸� ���� �� ���� ������
	PKT_S_TEST pkt;
	br >> pkt;

	// pkt�� �������� �����Ǿ����� üũ �մϴ�. 
	if (pkt.Validate() == false)
		return;

	// ���������� �Ľ�
	// ������ �׸�
	vector<PKT_S_TEST::BuffsListItem> buffs; 

	// ���� buffCount�� �����°� �ƴ� pkt���� �ѹ��� ������
	buffs.resize(pkt.buffCount);

	// ���������͸� ������ PKT_S_TEST �� �̸� �˷��� BuffsListItem ����ü��ŭ���� ������ �ֽ��ϴ�.
	for (int32 i = 0; i < pkt.buffCount; i++)
		br >> buffs[i];

	cout << "BuffCount : " << pkt.buffCount << endl;
	for (int32 i = 0; i < pkt.buffCount; i++)
	{
		cout << "Buff Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}

	// ���ڿ��� ���� �ǽ����� �ʰڽ��ϴ�.
}
