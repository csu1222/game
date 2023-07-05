#include "pch.h"
#include "BufferWriter.h"

/*
---------------------------
	BufferWriter 정의부
---------------------------
*/

BufferWriter::BufferWriter()
{
}

BufferWriter::BufferWriter(BYTE* buffer, uint32 size, uint32 pos)
	: _buffer(buffer), _size(size), _pos(pos)
{
}

BufferWriter::~BufferWriter()
{
}

bool BufferWriter::Write(void* src, uint32 len)
{
	// 남은 공간이 len 보다 작으면 작성할 공간이 없는것
	if (FreeSize() < len)
		return false;

	::memcpy(&_buffer[_pos], src, len);
	_pos += len;
	return true;
}

