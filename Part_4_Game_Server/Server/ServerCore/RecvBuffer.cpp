#include "pch.h"
#include "RecvBuffer.h"



/*
-------------------------
	RecvBuffer ���Ǻ�
-------------------------
*/

RecvBuffer::RecvBuffer(int32 bufferSize) : _bufferSize(bufferSize)
{
	// �������� ���ڸ�ŭ ���� ����� �������ݴϴ�. 
	_capacity = bufferSize * BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	// �б� ���Ⱑ ����Ǹ鼭 �۾��� �Ϸ�Ǿ��ų� ���ۿ� ���� ������ ���ڶ�� ��Ȳ�϶� 
	// Ŀ���� �����͵��� ���� ó������ �ٽ� �����ϵ��� �մϴ�. 

	// DataSize�� _readPos�� _writePos������ �����Դϴ�. 
	int32 dataSize = DataSize();
	if (dataSize == 0)
	{
		// ���� dataSize�� 0�̶�� write�Ѹ�ŭ read�� �Ϸ�� ��Ȳ�Դϴ�. 
		_readPos = _writePos = 0;
	}
	else
	{
		// Clean ���� ��� �Դµ� read,write ��ǥ�� ��ġ ���� �ʾҴٴ°��� 
		// �����Ϳ� Ŀ������ ���� ������ ���ܾ� �Ѵٴ� ���Դϴ�. 

		// ���� �����͸� ����� �ڵ��Դϴ�.
		//::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
		//_readPos = 0;
		//_writePos = dataSize;
		

		// ���� ������ �ϸ� ����� �ڵ尡 ���ٵ� �� �� ���ɻ� ������ ������ �ֽ��ϴ�. 
		// TCP����̴� ���� �� ��Ŷ��ŭ�� ���۵Ǵ°��� �ƴϰ� �Ϻ� �����Ͱ� �߷��� 
		// ���� �� ���� �ֽ��ϴ�. �׷��� ���� ���ۿ� ������ �־ ���� ���� �����ʹ� �ϴ� 
		// 0�� �ε����� �̵�������� �ϴ� ��Ȳ�� ����ϴ�. 
		// ������ �̵��� ���� ����� �߻��ϴµ� �̰��� �ִ��� �ٿ��ִ� ����� �ֽ��ϴ�. 
		// ó������ ���� ����� ���� �γ��ϰ� ��� �� Clean �Լ��� �̷Ｍ �ִ��� 
		// memcpy ���� Ŀ���� �ű� �� �ֵ��� �����ϴ°��Դϴ�. 

		// �׷��� ������ ���� ������ ���� 1�� ũ�� �̸��̸� �����͸� ������ ����ϴ�.
		if (FreeSize() < _bufferSize)
		{
			::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}

	}
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	// ���������� �����͸� ���� �� _readPos Ŀ���� ������ �����ݴϴ�. 

	// ���� ���� ó���� ������ numOfBytes�� ���� ������ ũ�� �Դϴ�. 
	// numOfBytes���� _writePos, _readPos�� ���� ������ ũ�ٴ°��� ���� �̻��� ��Ȳ�Դϴ�.
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	// OnRead�� ����ѵ� ���������� �����͸� ������ _writePos�� ������ ����ϴ�. 

	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}
