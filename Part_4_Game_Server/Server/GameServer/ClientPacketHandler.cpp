#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

/*
--------------------------
	ClientPacketHandler
--------------------------
*/

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : ���� �α�

	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// ������ session �� GameSession���� ĳ������ ���ڽ��ϴ�.
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation üũ 

	// ���� ���� ȸ��
	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// TODO :  DB���� �÷��̾� ������ �ܾ�´� 
	// TODO :  �÷��̾� ������ GameSession�� ����(�޸𸮿� ����)

	// ID �߱� (DB ID�� �ƴ� �ΰ��� ID)
	static Atomic<uint64> idGenerator = 1;

	{
		// ��Ŷ�� �� �÷��̾����� ä���
		auto player = loginPkt.add_players();
		player->set_name(u8"DB�����ܾ���̸�1");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		// �޸𸮻󿡼� ��� ���� �÷��̾�����
		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}
	{
		// ��Ŷ�� �� �÷��̾����� ä���
		auto player = loginPkt.add_players();
		player->set_name(u8"DB�����ܾ���̸�2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		// �޸𸮻󿡼� ��� ���� �÷��̾�����
		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	// TODO : Validate

	PlayerRef player = gameSession->_players[index];	// READ_ONLY?
	GRoom.Enter(player);		// WRITE_LOCK

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	player->ownerSession->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	// ���� �ʿ��� �α׸� ��� ���ϴ�. 
	cout << pkt.msg() << endl;

	// ���� �޼����� �� Ŭ���̾�Ʈ���� �ٽ� ���� 
	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GRoom.Broadcast(sendBuffer);	// WRITE_LOCK

	return true;
}