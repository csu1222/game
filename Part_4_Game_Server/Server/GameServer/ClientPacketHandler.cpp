#include "pch.h"
#include "ClientPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

/*
--------------------------
	ClientPacketHandler
--------------------------
*/

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : 실패 로그

	return false;
}


bool Handle_C_TEST(PacketSessionRef& session, Protocol::C_TEST& pkt)
{

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	return true;
}
