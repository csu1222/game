#pragma once

/*
--------------------------
	ServerPacketHandler
--------------------------
*/
enum
{
	S_TEST = 1,
};

// 가변 데이터 예시 
struct BuffData
{
	uint64 buffId;
	float remainTime;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack, /*가변데이터 실습*/vector<BuffData> buffs, /*UFT-16 문자열*/ wstring name);
};

