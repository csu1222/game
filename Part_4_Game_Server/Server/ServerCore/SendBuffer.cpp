#include "pch.h"
#include "SendBuffer.h"

/*
-------------------------
	SendBuffer 정의부
-------------------------
*/

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	// 복사하려는 데이터 크기가 버퍼사이즈를 넘어 오버 플로우가 일어나는지 체크
	ASSERT_CRASH(Capacity() >= len);

	// 어찌 됐건 버퍼에 데이터를 저장하려면 한번은 복사를 해야합니다. 
	::memcpy(_buffer.data(), data, len);
	_writeSize = len;
}
