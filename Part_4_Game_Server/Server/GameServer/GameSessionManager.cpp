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
	// �Ŵ����� �����ϴ� ��� ���ǿ� ���� Send�� ������ �ְ� �ֽ��ϴ�. 
	// �������� Send�� ���ڷ� �����͸� ���� �Ѱ��ְ� ���ο��� SendBuffer�� ���縦 �Ź� ������µ�
	// ������ ���ڷ� �Ѱ��ֱ� ���� �ѹ��� ���簡 �Ͼ�� �� �� ���ʹ� �� �ּҸ��� �Ѱ��ְ� �ֽ��ϴ�.
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}
