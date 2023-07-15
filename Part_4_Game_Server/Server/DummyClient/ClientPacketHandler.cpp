#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
// #include "Session.h" 는 CorePch에 추가 하였습니다. 
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
	// 여기서는 거꾸로 역직렬화 Deserialize를 합니다.
	Protocol::S_TEST pkt;

	// Parse 계열 함수가 아마 받은 버퍼를 읽는 기능 같습니다. 
	// 버퍼에서 PacketHeader를 뺀부분을 pkt에 담습니다. 
	ASSERT_CRASH(pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)));

	// 잘 파싱 되었는지 고정 데이터들을 출력해보겠습니다. 
	cout << pkt.id() << " " << pkt.hp() << " " << pkt.attack() << endl;

	// 가변 데이터인 buffs의 크기를 출력해보겠습니다. 

	cout << "BUFF_SIZE : " << pkt.buffs_size() << endl;

	// buffs를 꺼내 순회 문법이 먹히는지 보겠습니다. 
	for (auto& buff : pkt.buffs())
	{
		cout << "BUFF_ID : " << buff.buffid() << " BUFF_REMAINTIME : " << buff.remaintime() << endl;
		// victims 데이터도 확인하겠습니다. 
		cout << "VICTIMS_SIZE : " << buff.victims_size() << endl;
		for (auto& victim : buff.victims())
		{
			// victim은 uint64 타입이다 보니 바로 출력할 수 있습니다. 
			cout << victim << " " << endl;
		}
	}
}