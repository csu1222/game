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

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

// 패킷 설계 TEMP
struct S_TEST
{
	uint64 id;
	uint32 hp;
	uint16 attack;
	// 가변데이터 
	vector<BuffData> buffs;

	// UTF-16 사용 문자열 
	wstring name;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	// 패킷을 처리만 하는 함수이긴하지만 header 까지 꺼내줘야 순서가 맞습니다. 
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	uint64 id;
	uint32 hp;
	uint16 attack;

	br >> id >> hp >> attack;

	cout << "ID :" << id << " HP :" << hp << " ATT:" << attack << endl;


	// 가변데이터 파싱
	// 추출할 그릇
	vector<BuffData> buffs; 

	uint16 buffCount;

	// 패킷에 담았던 순서 그대로 꺼냅니다.
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

	// 문자열을 받아줄 그릇
	// Make_S_Test에서와 똑같은 순서로 데이터를 꺼냈습니다. 
	wstring name;
	uint16 nameLen;
	br >> nameLen;
	name.resize(nameLen);

	br.Read((void*)name.data(), nameLen * sizeof(WCHAR));
	
	// wstring 을 사용하면서 wcout 을 사용해야 한다거나 추가 설정을 해야하는것이 있습니다
	wcout.imbue(std::locale("kor"));
	wcout << name << endl;
}
