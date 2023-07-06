#pragma once

/*
---------------------------
	ClientPacketHandler
---------------------------
*/
enum
{
	// 서버에서 클라로 보내는 패킷은 S_ 를 붙힙니다. 
	S_TEST = 1,
};

// ServerSession::OnRecvPacket에서 패킷을 처리하던걸 여기서 대신 합니다.
class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_S_TEST(BYTE* buffer, int32 len);
};

