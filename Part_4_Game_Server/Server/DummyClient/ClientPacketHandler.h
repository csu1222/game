#pragma once

/*
---------------------------
	ClientPacketHandler
---------------------------
*/
enum
{
	S_TEST = 1,
};

class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_S_TEST(BYTE* buffer, int32 len);
};


template<typename T, typename C>
class PacketIterator
{
public:
	PacketIterator(C& container, uint16 index) : _container(container), _index(index) { }

	bool				operator!=(const PacketIterator& other) const { return _index != other._index; }
	const T&			operator*() const { return _container[_index]; }
	T&					operator*() { return _container[_index]; }
	T*					operator->() { return &_container[_index]; }
	PacketIterator&		operator++ () { _index++; return *this; } // 전위++
	PacketIterator		operator++(int32) { PacketIterator ret = *this; ++_index; return ret; }
private:
	C&					_container;
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
	T*				_data;
	uint16			_count;
};