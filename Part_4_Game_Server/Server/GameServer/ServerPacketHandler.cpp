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
	// ClientPacketHandler ������ >> �����ڷ� �׳� ���¾��µ� 
	// ��� header�� id�� ���캸�� �Ǻ��ϸ� �Ǵϱ� Peek() �� ����ص� �˴ϴ�.
	br.Peek(&header);

	// ����� Ŭ���̾�Ʈ���� ��Ŷ�� �޵��� ���������� ���� ������ ���� ���ٰ� �����ϴ�. 
	switch (header.id)
	{
	default:
		break;
	}
}

