#pragma once
#include "Session.h"

/*
-------------------------
	GameSession ����� 
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
	// OnRecv�� sealed�� ��ɰ� ��� OnRecvPacket�� �������� ����մϴ�. 
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};

