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

int32 GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// ������ OnRecvPacket���� ���� �����ʹ� ������ ��Ŷ�̶�°� ���� ���� �� �ֽ��ϴ�. 

	// �̰����� �׽�Ʈ�� �Ұ����� ���� ���� ��Ŷ�� ��� ������ �����غ��ڽ��ϴ�. 
	PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[0]));

	cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

	// �̹� �׽�Ʈ������ Echo ������ �ƴ� GameServer.cpp �ʿ��� ��ü������ �ѷ��ְڽ��ϴ�. 

	//SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
	//::memcpy(sendBuffer->Buffer(), buffer, len);
	//sendBuffer->Close(len);
	//
	//// �����͸� �޾����� �ڽ� ����� �ִ� Ŭ��Ӹ� �ƴ϶� ���� �Ŵ����� ��� ���ǿ� Send
	//GSessionManager.Broadcast(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
}