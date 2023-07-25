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
	// TODO : 실패 로그

	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// 인자의 session 을 GameSession으로 캐스팅해 보겠습니다.
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation 체크 

	// 입장 성공 회답
	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// TODO :  DB에서 플레이어 정보를 긁어온다 
	// TODO :  플레이어 정보를 GameSession에 저장(메모리에 저장)

	// ID 발급 (DB ID가 아닌 인게임 ID)
	static Atomic<uint64> idGenerator = 1;

	{
		// 패킷에 줄 플레이어정보 채우기
		auto player = loginPkt.add_players();
		player->set_name(u8"DB에서긁어온이름1");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		// 메모리상에서 들고 있을 플레이어정보
		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}
	{
		// 패킷에 줄 플레이어정보 채우기
		auto player = loginPkt.add_players();
		player->set_name(u8"DB에서긁어온이름2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		// 메모리상에서 들고 있을 플레이어정보
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
	// 서버 쪽에서 로그를 찍어 봅니다. 
	cout << pkt.msg() << endl;

	// 받은 메세지를 각 클라이언트에게 다시 전달 
	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GRoom.Broadcast(sendBuffer);	// WRITE_LOCK

	return true;
}