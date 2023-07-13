#pragma once
#include "BufferReader.h"
#include "BufferWriter.h"

/*
--------------------------
	ServerPacketHandler
--------------------------
*/
enum
{
	S_TEST = 1,
};



template<typename T, typename C>
class PacketIterator
{
public:
	PacketIterator(C& container, uint16 index) : _container(container), _index(index) { }

	bool				operator!=(const PacketIterator& other) const { return _index != other._index; }
	const T& operator*() const { return _container[_index]; }
	T& operator*() { return _container[_index]; }
	T* operator->() { return &_container[_index]; }
	PacketIterator& operator++ () { _index++; return *this; } // 전위++
	PacketIterator		operator++(int32) { PacketIterator ret = *this; ++_index; return ret; }
private:
	C& _container;
	uint16				_index;
};

template<typename T>
class PacketList
{
public:
	PacketList() : _data(nullptr), _count(0) { }
	PacketList(T* data, uint16 count) : _data(data), _count(count) { }

	T& operator[] (uint16 index)
	{
		ASSERT_CRASH(index < _count);
		return _data[index];
	}

	uint16 Count() { return _count; }

	// ranged-based for 지원
	PacketIterator<T, PacketList<T>> begin() { return PacketIterator<T, PacketList<T>>(*this, 0); }
	PacketIterator<T, PacketList<T>> end() { return PacketIterator<T, PacketList<T>>(*this, _count); }
private:
	// 데이터 타입(T)과 시작주소(_data), 그리고 갯수(_count)를 알면 
	// 그냥 배열처럼 접근 할 수 있을것입니다. 
	T* _data;
	uint16			_count;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

};

#pragma pack(1)
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;

		// victims List
		uint16 victimsOffset;
		uint16 victimsCount;
	};

	uint16 packetSize; // 공용 헤더
	uint16 packetId; // 공용 헤더
	uint64 id; // 8
	uint32 hp; // 4
	uint16 attack; // 2
	uint16 buffsOffset;
	uint16 buffsCount;
};


// 데이터를 쓰기 위한 클래스 
class PKT_S_TEST_WRITE
{
public:
	using BuffsListItem = PKT_S_TEST::BuffsListItem;
	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
	using BuffsVictimsList = PacketList<uint64>; // victim 추가는 굳이 struct를 만들지 않고 총크기인 uint64로 만들겠습니다.  


	// 고정데이터들을 인자로 받는 생성자 
	PKT_S_TEST_WRITE(uint64 id, uint32 hp, uint16 attack)
	{
		// 이전 Make_S_TEST 에서 해주던 코드를 생성자에서 합니다. 
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());


		// PKT_S_TEST를 예약해두고 커서는 가변 데이터의 시작 위치로 옮깁니다. 
		_pkt = _bw.Reserve<PKT_S_TEST>();
		_pkt->packetSize = 0; // 패킷의 전체 사이즈는 아직 채워줄 수 없습니다. to Fill
		_pkt->packetId = S_TEST;
		_pkt->id = id;
		_pkt->hp = hp;
		_pkt->attack = attack;
		_pkt->buffsOffset = 0; // 처음엔 일단 0으로 둡니다.
		_pkt->buffsCount = 0; // 버프의 갯수도 아직 모릅니다. 

		// 가변데이터가 개입될 부분은 아직 모르지만 일단 고정 데이터들은 생성자에서 채워줬습니다.
	}

	// 가변데이터인 BuffsList를 채워줄 함수
	BuffsList ReserveBuffsList(uint16 buffCount)
	{
		// vector나 list 같은 STL 컨테이너들은 실시간으로 각각의 데이터 들을 추가할 수 있었습니다.
		// 하지만 지금 버퍼를 채우는 방식에서는 실시간 데이터 추가는 어렵습니다. 
		// 가변길이 데이터가 두 종류만 되더라도 다른종류의 가변데이터가 섞이면서 문제가 됩니다.
		// 그래서 버퍼를 곧바로 채우는 방식에서는 이 함수처럼 가변데이터의 길이를 딱 지정한 다음 진행하는 경우가 많습니다.
		// BufferWriter::Reserve가 count를 받는 식으로 수정했습니다. 
		BuffsListItem* firstBuffListItem = _bw.Reserve<BuffsListItem>(buffCount);
		
		// 생성자때 채워주지 못했던 데이터를 채워줍니다.
		// buffOffset은 _pkt의 시작주소에서부터 가변데이터가 시작하는 주소까지의 인덱스 같은것이었습니다. 
		_pkt->buffsOffset = (uint64)firstBuffListItem - (uint64)_pkt;
		_pkt->buffsCount = buffCount;

		// 반환은 예약된 버퍼를 BuffsList로 만들어 반환합니다. 
		return BuffsList(firstBuffListItem, buffCount);
	}

	BuffsVictimsList ReserveBuffsVictimsList(BuffsListItem* buffsItem, uint16 victimsCount)
	{
		uint64* firstVictimsListItem = _bw.Reserve<uint64>(victimsCount);
		buffsItem->victimsOffset = (uint64)firstVictimsListItem - (uint64)_pkt;
		buffsItem->victimsCount = victimsCount;

		return BuffsVictimsList(firstVictimsListItem, victimsCount);
	}
	
	// 데이터들을 다 밀어 넣은 다음 마무리하는 함수
	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈를 최종 계산 
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());

		return _sendBuffer;
	}

private:
	// 버퍼를 쓸데 필요한 객체들을 들고 있겠습니다.
	PKT_S_TEST* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};

#pragma pack()