#pragma once
#include "Protocol.pb.h"

/*
--------------------------
	ServerPacketHandler
--------------------------
*/

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

// TODO : 자동화 
enum : uint16
{
	PKT_S_TEST = 1,
	PKT_S_LOGIN = 2,
};

// TODO : 자동화 (선언까지만)
// Custom Handler
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt);

class ServerPacketHandler
{
public:
	// TODO : 자동화 
	static void Init()
	{
		for(int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;

		GPacketHandler[PKT_S_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TEST>(Handle_S_TEST, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		// 헤더의 아이디를 인덱스로 바로 Handler 함수를 호출합니다. 
		return GPacketHandler[header->id](session, buffer, len);
	}

	// TODO : 자동화
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
		// 인자로 받은 객체의 사이즈를 가져옵니다. 
		const uint16 dataSize = pkt.ByteSizeLong();
		// 패킷 자체의 크기에는 데이터 뿐만아니라 패킷 헤더의 크기 까지 포함됩니다.
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		// SendBuffer를 open 해주겠습니다.		
		// 이전에는 그냥 4096 이라는 큰사이즈를 할당한 다음 데이터를 채우고 close를 해줬습니다.
		// 그런데 지금은 딱 할당받아야하는 packetSize를 계산할 수 있습니다.
		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);

		// 버퍼의 내용을 채웁니다. 
		// 먼저 패킷헤더의 내용을 채웁니다. 
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;

		// 헤더다음 4바이트 이후 부터 패킷을 serialize 해서 넣어주면 되었습니다. 
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));

		// sendBuffer를 닫고 리턴합니다. 
		sendBuffer->Close(packetSize);
		return sendBuffer;
	}
};