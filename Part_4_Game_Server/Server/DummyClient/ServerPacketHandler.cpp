#include "pch.h"
#include "ServerPacketHandler.h"


/*
---------------------------
	ServerPacketHandler
---------------------------
*/

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : 실패 로그

	return false;
}

bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt)
{
	cout << pkt.id() << " " << pkt.hp() << " " << pkt.attack() << endl;

	cout << "BUFSIZE : " << pkt.buffs_size() << endl;

	for (auto& buff : pkt.buffs())
	{
		cout << "BUFINFO : " << buff.buffid() << " " << buff.remaintime() << endl;
		cout << "VICTIMS : " << buff.victims_size() << endl;
		for (auto& victim : buff.victims())
		{
			cout << "VICTIM INFO : " << victim << endl;
		}
		cout << endl;
	}

	return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	return true;
}