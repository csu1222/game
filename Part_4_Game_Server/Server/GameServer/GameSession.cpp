#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"

/*
-------------------------
	GameSession 정의부
-------------------------
*/

void GameSession::OnConnected()
{
	// 접속과 동시에 세션 매니저에 등록
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	// 접속과 반대 
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	ServerPacketHandler::HandlePacket(buffer, len);
}

void GameSession::OnSend(int32 len)
{
}