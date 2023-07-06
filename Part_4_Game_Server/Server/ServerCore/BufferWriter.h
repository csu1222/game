#pragma once

/*
---------------------------
	BufferWriter 선언부
---------------------------
*/

// BufferReader 와 비슷한 부분이 많습니다. 
class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE*				Buffer() { return _buffer; }
	uint32				Size() { return _size; }
	uint32				WriteSize() { return _pos; }
	uint32				FreeSize() { return _size - _pos; }

	// 버퍼에 소스(src)를 작성합니다. 
	template<typename T>
	bool				Write(T* src) { return Read(src, sizeof(T)); }
	bool				Write(void* src, uint32 len);

	// 버퍼를 작성할때 헤더를 가장 마직막에 작성할때가 있는데 그때 사용할 함수
	template<typename T>
	T*					Reserve();

	// 데이터를 밀어 넣는다는 뜻의 << 연산자 
	// 템플릿을 사용하면 보편 참조 버전이었습니다. 
	template<typename T>
	BufferWriter& operator<<(T&& src);

private:
	// 멤버 변수는 BufferReader와 똑같습니다. 
	BYTE*				_buffer = nullptr;	// 버퍼의 시작주소 
	uint32				_size = 0;			// 버퍼의 사이즈
	uint32				_pos = 0;			// 어디까지 썻는가
};

template<typename T>
T* BufferWriter::Reserve()
{
	// 예약할 T타입 만큼도 여유가 없다면 실패
	if (FreeSize() < sizeof(T))
		return nullptr;

	// 여유가 있다면 현재위치의 포인터를 ret 에 담아 반환 할것
	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	// 다만 T 만큼 커서는 당겨줄것
	_pos += sizeof(T);

	return ret;
}


template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	using DataType = std::remove_reference_t<T>;
	*reinterpret_cast<DataType*>(&_buffer[_pos]) = std::forward< DataType>(src);
	_pos += sizeof(T);
	return *this;
}
