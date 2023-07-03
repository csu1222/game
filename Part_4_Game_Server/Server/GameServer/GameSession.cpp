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

int32 GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// 이제는 OnRecvPacket으로 받은 데이터는 온전한 패킷이라는걸 보장 받을 수 있습니다. 

	// 이것저것 테스트를 할것인제 먼저 받은 패킷의 헤더 정보를 추출해보겠습니다. 
	PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[0]));

	cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

	// 이번 테스트에서는 Echo 서버가 아닌 GameServer.cpp 쪽에서 전체적으로 뿌려주겠습니다. 

	//SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
	//::memcpy(sendBuffer->Buffer(), buffer, len);
	//sendBuffer->Close(len);
	//
	//// 데이터를 받았으면 자신 연결되 있는 클라뿐만 아니라 세션 매니저의 모든 세션에 Send
	//GSessionManager.Broadcast(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
}