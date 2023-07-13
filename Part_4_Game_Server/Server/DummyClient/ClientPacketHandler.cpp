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

		// 다중 리스트 
		uint16 victimsOffset;
		uint16 victimsCount;

		// BuffsListItem 내부에서도 사이즈 비교를 하고 잘왔는지를 bool로 반환하고 인자인 size에다가
		// 확인한 데이터 크기를 더해줄겁니다. 
		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			// victims 까지 합한 데이터 사이즈가 packetSize보다 크다면 문제가 있습니다.
			if (victimsOffset + victimsCount * sizeof(uint64) > packetSize)
				return false;

			// 통과했다면 패킷의 사이즈에 더해줍니다.
			size += victimsCount * sizeof(uint64);
			return true;			
		}
	};

	uint16 packetSize;
	uint16 packetId;
	uint64 id;
	uint32 hp;
	uint16 attack;
	// 가변길이 데이터를 묘사하는 데이터 
	uint16 buffOffset; // 가변길이 데이터가 시작하는 인덱스
	uint16 buffCount;
	
	// 패킷의 내용이 정상적으로 처리되고 있는지를 확인하는 함수 
	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_TEST);
		// 이 체크는 적어도 고정데이터들은 들어와 있는지를 확인
		if (packetSize < size)
			return false;

		// Buffs 가변데이터 크기 추가 
		if (buffOffset + buffCount * sizeof(BuffsListItem) > packetSize)
			return false;
		
		size += buffCount * sizeof(BuffsListItem);

		// BuffsList를 순회하면서 victims 데이터의 크기를 확인합니다. 
		BuffsList buffsList = GetBuffsList();
		for (int32 i = 0; i < buffsList.Count(); i++)
		{
			if (buffsList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;
		}

		// 최종 크기 비교
		if (size != packetSize)
			return false;


		return true;
	}

	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
	using BuffsVictimsList = PacketList<uint64>;

	// BuffsList를 꺼내는 함수
	BuffsList GetBuffsList()
	{
		// PKT_S_TEST의 시작주소에 buffOffset 만큼을 더하면 가변데이터의 시작 주소입니다. 
		// BYTE* 가 1바이트짜리니까 uint16인 buffOffset을 더하면 주소가 나옵니다.
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffOffset;

		// 반환할때는 다시 PKT_S_TEST::BuffsListItem*로 캐스팅해서 PakcetList의 생성자에 넘겨줍니다.
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

	// 버퍼의 데이터를 굳이 임시 객체에 복사하지 않아도 되지 않을까?

	// BYTE 배열인 buffer를 바로 캐스팅해서 PKT_S_TEST로 해석을 합니다.
	PKT_S_TEST* pkt = reinterpret_cast<PKT_S_TEST*>(buffer);

	// PacketSession::OnRecv를 지나오면서 일단 PacketHeader 만큼은 들어 있다고 보장할 수 있습니다. 
	// 그 헤더에 적힌 사이즈와 버퍼크기를 비교하기는 했는데 그게 오염되어 있는지는 확인해봐야합니다. 

	// Validate 에서 순서대로 고정데이터까지는 들어왔는지 들어왔다면 그 내용들을 가지고 가변데이터들 까지 제대로 들어왔는지를 
	// 확인합니다. 
	if (pkt->Validate() == false)
		return;

	// 여기까지 왔다면 일단 pkt 라는 포인터가 buffer에 있는 데이터들을 PKT_S_TEST로 해석합니다.

	// 그렇다면 이전처럼 가변데이터들도 굳이 새로 객체를 만들어 받아줄 필요없이 
	// 데이터들의 주소를 알수 있기때문에 곧바로 접근 할 수 있을겁니다. 
	// 데이터에 접근하기 편하게 헬퍼 클래스를 만들었습니다.

	// 가변데이터를 PacketList로 관리해 줍니다. 

	PKT_S_TEST::BuffsList buffs = pkt->GetBuffsList();

	// buffs는 buffer의 데이터를 복사한게 아닌 그냥 포인터로 가리키고 있는것입니다. 

	// 따로 꺼내 저장하는게 아닌 PacketList에 만들어둔 [] 연산자로 직접 접근할수 있게 됩니다. 
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
