#pragma once
#include "Protocol.pb.h"

/*
--------------------------
	ServerPacketHandler
--------------------------
*/

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

// TODO : �ڵ�ȭ 
enum : uint16
{
	PKT_S_TEST = 1,
	PKT_S_LOGIN = 2,
};

// TODO : �ڵ�ȭ (���������)
// Custom Handler
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt);

class ServerPacketHandler
{
public:
	// TODO : �ڵ�ȭ 
	static void Init()
	{
		for(int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;

		GPacketHandler[PKT_S_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TEST>(Handle_S_TEST, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		// ����� ���̵� �ε����� �ٷ� Handler �Լ��� ȣ���մϴ�. 
		return GPacketHandler[header->id](session, buffer, len);
	}

	// TODO : �ڵ�ȭ
	static SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt) { return MakeSendBuffer(pkt, PKT_S_TEST); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
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
};