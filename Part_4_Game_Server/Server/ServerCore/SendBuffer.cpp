#include "pch.h"
#include "SendBuffer.h"

/*
-------------------------
	SendBuffer ���Ǻ�
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
	// �����Ϸ��� ������ ũ�Ⱑ ���ۻ���� �Ѿ� ���� �÷ο찡 �Ͼ���� üũ
	ASSERT_CRASH(Capacity() >= len);

	// ���� �ư� ���ۿ� �����͸� �����Ϸ��� �ѹ��� ���縦 �ؾ��մϴ�. 
	::memcpy(_buffer.data(), data, len);
	_writeSize = len;
}
