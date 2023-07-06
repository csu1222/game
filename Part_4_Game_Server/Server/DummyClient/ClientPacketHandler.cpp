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

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

// ��Ŷ ���� TEMP
struct S_TEST
{
	uint64 id;
	uint32 hp;
	uint16 attack;
	// ���������� 
	vector<BuffData> buffs;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	// ��Ŷ�� ó���� �ϴ� �Լ��̱������� header ���� ������� ������ �½��ϴ�. 
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	uint64 id;
	uint32 hp;
	uint16 attack;

	br >> id >> hp >> attack;

	cout << "ID :" << id << " HP :" << hp << " ATT:" << attack << endl;


	// ���������� �Ľ�
	// ������ �׸�
	vector<BuffData> buffs; 

	uint16 buffCount;

	// ��Ŷ�� ��Ҵ� ���� �״�� �����ϴ�.
	br >> buffCount;

	buffs.resize(buffCount);
	for (int32 i = 0; i < buffCount; i++)
	{
		br >> buffs[i].buffId >> buffs[i].remainTime;
	}

	cout << "BuffCount : " << buffCount << endl;
	for (int32 i = 0; i < buffCount; i++)
	{
		cout << "Buff Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}
}
