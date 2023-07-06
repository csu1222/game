#pragma once
#include "Session.h"

/*
-------------------------
	GameSession 선언부 
-------------------------
*/
class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	// OnRecv는 sealed로 잠궜고 대신 OnRecvPacket을 재정의해 사용합니다. 
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};

