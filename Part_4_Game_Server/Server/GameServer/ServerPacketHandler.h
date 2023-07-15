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

	// _MakeSendBuffer 함수를 S_TEST 타입으로 사용하는 래핑 함수입니다.
	// 이 내용을 매번 패킷종류가 늘 때 마다 손수 만들어 주기 보다는 자동화를 하면 좋겠습니다.
	static SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt);
};


// Protocol로 데이터를 버퍼로 만드는 기능을 함수로 구현해보겠습니다. 
// 이 함수는 거의 공용으로 사용될겁니다. 
template<typename T>
SendBufferRef _MakeSendBuffer(T& pkt, uint16 pktId)
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