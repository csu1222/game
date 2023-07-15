#pragma once
#include "BufferReader.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"

/*
--------------------------
	ServerPacketHandler
--------------------------
*/
enum
{
	S_TEST = 1,
};



class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	// _MakeSendBuffer �Լ��� S_TEST Ÿ������ ����ϴ� ���� �Լ��Դϴ�.
	// �� ������ �Ź� ��Ŷ������ �� �� ���� �ռ� ����� �ֱ� ���ٴ� �ڵ�ȭ�� �ϸ� ���ڽ��ϴ�.
	static SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt);
};


// Protocol�� �����͸� ���۷� ����� ����� �Լ��� �����غ��ڽ��ϴ�. 
// �� �Լ��� ���� �������� ���ɰ̴ϴ�. 
template<typename T>
SendBufferRef _MakeSendBuffer(T& pkt, uint16 pktId)
{
	// ���ڷ� ���� ��ü�� ����� �����ɴϴ�. 
	const uint16 dataSize = pkt.ByteSizeLong();
	// ��Ŷ ��ü�� ũ�⿡�� ������ �Ӹ��ƴ϶� ��Ŷ ����� ũ�� ���� ���Ե˴ϴ�.
	const uint16 packetSize = dataSize + sizeof(PacketHeader);

	// SendBuffer�� open ���ְڽ��ϴ�. 
	// �������� �׳� 4096 �̶�� ū����� �Ҵ��� ���� �����͸� ä��� close�� ������ϴ�.
	// �׷��� ������ �� �Ҵ�޾ƾ��ϴ� packetSize�� ����� �� �ֽ��ϴ�.
	SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);

	// ������ ������ ä��ϴ�. 
	// ���� ��Ŷ����� ������ ä��ϴ�. 
	PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
	header->size = packetSize;
	header->id = pktId;

	// ������� 4����Ʈ ���� ���� ��Ŷ�� serialize �ؼ� �־��ָ� �Ǿ����ϴ�. 
	ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));

	// sendBuffer�� �ݰ� �����մϴ�. 
	sendBuffer->Close(packetSize);
	return sendBuffer;
}