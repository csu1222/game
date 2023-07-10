#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
// #include "Session.h" 는 CorePch에 추가 하였습니다. 


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

	// 패킷 id에 따라 처리를 해줍니다. 
	// 경우에 따라 진짜 패킷종류에 따라 수백개의 switch 문을 사용하기도 한답니다.

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	}
}

#pragma pack(1)
// 패킷 설계 TEMP
struct PKT_S_TEST
{
	// 가변길이 데이터의 사이즈 계산에 필요합니다.
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
	// 가변길이 데이터를 묘사하는 데이터 
	uint16 buffOffset; // 가변길이 데이터가 시작하는 인덱스
	uint16 buffCount;
	//vector<BuffData> buffs;
	//wstring name;
	
	// 패킷의 내용이 정상적으로 처리되고 있는지를 확인하는 함수 
	bool Validate()
	{
		// 길이 테스트를 하면됩니다. 
		uint32 size = 0;

		// 먼저 PKT_S_TEST의 사이즈를 더합니다.
		size += sizeof(PKT_S_TEST);

		// 가변길이 데이터의 사이즈를 더합니다. 
		size += buffCount * sizeof(BuffsListItem);

		// 이렇게 더한 사이즈가 헤더의 packetSize와 다르다고 하면 문제가 있습니다.
		if (size != packetSize)
			return false;

		// 가변길이 데이터의 시작 인덱스인 buffOffset의 값이 문제 없는지 다시 체크
		if (buffOffset + buffCount * sizeof(BuffsListItem) > packetSize)
			return false;

		return true;

	}
};
#pragma pack()

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	// 패킷을 처리만 하는 함수이긴하지만 header 까지 꺼내줘야 순서가 맞습니다. 
	BufferReader br(buffer, len);

	// 패킷안에 header에 해당하는 내용을 넣었습니다.
	//PacketHeader header;
	//br >> header;

	// 받은 데이터크기가 적어도 고정데이터는 들어 있는지 체크합니다. 
	if (len < sizeof(PKT_S_TEST))
		return;

	// 패킷에서 구조체 덩어리로 데이터를 꺼낼 수 있지 않을까
	PKT_S_TEST pkt;
	br >> pkt;

	// pkt가 문제없이 검증되었는지 체크 합니다. 
	if (pkt.Validate() == false)
		return;

	// 가변데이터 파싱
	// 추출할 그릇
	vector<PKT_S_TEST::BuffsListItem> buffs; 

	// 따로 buffCount를 꺼내는게 아닌 pkt에서 한번에 관리함
	buffs.resize(pkt.buffCount);

	// 가변데이터를 꺼낼때 PKT_S_TEST 에 미리 알려준 BuffsListItem 구조체만큼씩을 꺼낼수 있습니다.
	for (int32 i = 0; i < pkt.buffCount; i++)
		br >> buffs[i];

	cout << "BuffCount : " << pkt.buffCount << endl;
	for (int32 i = 0; i < pkt.buffCount; i++)
	{
		cout << "Buff Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}

	// 문자열은 아직 실습하지 않겠습니다.
}
