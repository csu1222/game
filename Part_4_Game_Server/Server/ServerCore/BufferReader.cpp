#include "pch.h"
#include "BufferReader.h"

/*
---------------------------
	BufferReader ���Ǻ�
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
	// ���� �а� ���� �����;� ���� ���������� ������ ����
	if (FreeSize() < len)
		return false;

	// ������ �ǵ帮�� �ʰ� ���ڷ� �� dest�� �����մϴ�. 
	::memcpy(dest, &_buffer[_pos], len);

	return true;
}

bool BufferReader::Read(void* dest, uint32 len)
{
	// Peek ó�� dest ��ġ�� ���� Ŀ������ len ��ŭ�� �����͸� �������Դϴ�. 
	if (Peek(dest, len) == false)
		return false;

	// �о��ٸ� �׸�ŭ Ŀ���� ����ϴ�. 
	_pos += len;

	return true;
}
