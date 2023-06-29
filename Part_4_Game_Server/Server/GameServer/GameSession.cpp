#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"

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

int32 GameSession::OnRecv(BYTE* buffer, int32 len)
{
	cout << "OnRecv Len = " << len << endl;

	SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096);
	sendBuffer->CopyData(buffer, len);
	
	// 데이터를 받았으면 자신 연결되 있는 클라뿐만 아니라 세션 매니저의 모든 세션에 Send
	GSessionManager.Broadcast(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
	cout << "OnSend Len = " << len << endl;
}