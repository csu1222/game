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
	// TODO : ���� �α�

	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	// ���� ��Ŷ üũ 
	if (pkt.success() == false)
		return true;

	if (pkt.players().size() == 0)
	{
		// ĳ���� ����â
	}

	// ���� UI ��ư ������ ���� ���� 
	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playerindex(0);	// ù��° ĳ���ͷ� ���� 

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	cout << pkt.msg() << endl;
	return true;
}
