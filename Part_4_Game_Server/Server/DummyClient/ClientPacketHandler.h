#pragma once

/*
---------------------------
	ClientPacketHandler
---------------------------
*/
enum
{
	// �������� Ŭ��� ������ ��Ŷ�� S_ �� �����ϴ�. 
	S_TEST = 1,
};

// ServerSession::OnRecvPacket���� ��Ŷ�� ó���ϴ��� ���⼭ ��� �մϴ�.
class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_S_TEST(BYTE* buffer, int32 len);
};

