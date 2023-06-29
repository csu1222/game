#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

GameSessionManager GSessionManager;

void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	// 매니저가 관리하는 모든 세션에 대해 Send를 실행해 주고 있습니다. 
	// 이전에는 Send의 인자로 데이터를 직접 넘겨주고 내부에서 SendBuffer에 복사를 매번 해줬었는데
	// 지금은 인자로 넘겨주기 전에 한번만 복사가 일어나고 이 후 부터는 그 주소만을 넘겨주고 있습니다.
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}
