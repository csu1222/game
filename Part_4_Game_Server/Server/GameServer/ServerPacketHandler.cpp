#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

/*
--------------------------
	ServerPacketHandler
--------------------------
*/
void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	// ClientPacketHandler 에서는 >> 연산자로 그냥 꺼냈었는데 
	// 사실 header의 id만 살펴보고 판별하면 되니까 Peek() 을 사용해도 됩니다.
	br.Peek(&header);

	// 현재는 클라이언트에서 패킷을 받도록 설계한적이 없기 때문에 딱히 해줄게 없습니다. 
	switch (header.id)
	{
	default:
		break;
	}
}

