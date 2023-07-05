#pragma once

/*
---------------------------
	BufferReader 선언부 
---------------------------
*/

// BufferReader의 역할은 버퍼의 시작주소와 그 버퍼의 크기를 받아 관리하는 역할
class BufferReader
{
public:
	BufferReader();
	BufferReader(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferReader();

	BYTE*				Buffer() { return _buffer; }
	uint32				Size() { return _size; }
	uint32				ReadSize() { return _pos; }
	uint32				FreeSize() { return _size - _pos; }
	
	// Peek을 좀 더 사용하기 쉽게 
	template<typename T>
	bool				Peek(T* dest) { return Peek(dest, sizeof(T)); }
	// Peek은 커서를 옮기지는 않고 잠깐 엿보고만 싶을때 
	bool				Peek(void* dest, uint32 len);

	// 내용도 확인하고 커서도 앞으로 옮깁니다. 
	template<typename T>
	bool				Read(T* dest) { return Read(dest, sizeof(T)); }
	bool				Read(void* dest, uint32 len);

	// 자주 사용하던 cout << 의 << 도 연산자 오버로딩 대상이었습니다.
	template<typename T>
	BufferReader&		operator>>(OUT T& dest);

private:
	BYTE*				_buffer = nullptr;	// 버퍼의 시작주소 
	uint32				_size = 0;			// 버퍼의 사이즈
	uint32				_pos = 0;			// 지금 어디까지 읽었는가
};

template<typename T>
inline BufferReader& BufferReader::operator>>(OUT T& dest)
{
	// >> 연산자는 BufferReader에서 지금까지 읽은 위치에서 내용물을꺼내 
	// dest에 담아 준다는 의미 

	// _buffer[_pos]의 위치의 데이터를 T타입으로 캐스팅해서 dest에 담습니다. 
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);
	// dest에 담긴 데이터만큼을 읽었다고 치고 커서를 옮깁니다. 
	_pos += sizeof(T);
	return *this;
}