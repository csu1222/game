#include "pch.h"
#include "BufferReader.h"

/*
---------------------------
	BufferReader 정의부
---------------------------
*/

BufferReader::BufferReader()
{
}

BufferReader::BufferReader(BYTE* buffer, uint32 size, uint32 pos)
	: _buffer(buffer), _size(size), _pos(pos)
{
}

BufferReader::~BufferReader()
{
}

bool BufferReader::Peek(void* dest, uint32 len)
{
	// 내가 읽고 싶은 데이터양 보다 여유공간이 적으면 실패
	if (FreeSize() < len)
		return false;

	// 원본은 건드리지 않고 인자로 준 dest에 복사합니다. 
	::memcpy(dest, &_buffer[_pos], len);

	return true;
}

bool BufferReader::Read(void* dest, uint32 len)
{
	// Peek 처럼 dest 위치에 현재 커서부털 len 만큼의 데이터를 읽을것입니다. 
	if (Peek(dest, len) == false)
		return false;

	// 읽었다면 그만큼 커서도 옯깁니다. 
	_pos += len;

	return true;
}
