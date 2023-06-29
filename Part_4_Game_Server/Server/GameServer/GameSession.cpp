#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"

/*
-------------------------
	GameSession ���Ǻ�
-------------------------
*/

void GameSession::OnConnected()
{
	// ���Ӱ� ���ÿ� ���� �Ŵ����� ���
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	// ���Ӱ� �ݴ� 
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

int32 GameSession::OnRecv(BYTE* buffer, int32 len)
{
	cout << "OnRecv Len = " << len << endl;

	SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096);
	sendBuffer->CopyData(buffer, len);
	
	// �����͸� �޾����� �ڽ� ����� �ִ� Ŭ��Ӹ� �ƴ϶� ���� �Ŵ����� ��� ���ǿ� Send
	GSessionManager.Broadcast(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
	cout << "OnSend Len = " << len << endl;
}