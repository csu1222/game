#include "pch.h"
#include "RecvBuffer.h"



/*
-------------------------
	RecvBuffer 정의부
-------------------------
*/

RecvBuffer::RecvBuffer(int32 bufferSize) : _bufferSize(bufferSize)
{
	// 성성자의 인자만큼 버퍼 사이즈를 설정해줍니다. 
	_capacity = bufferSize * BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	// 읽기 쓰기가 진행되면서 작업이 완료되었거나 버퍼에 여유 공간이 모자라는 상황일때 
	// 커서와 데이터들을 버퍼 처음부터 다시 시작하도록 합니다. 

	// DataSize는 _readPos와 _writePos사이의 간격입니다. 
	int32 dataSize = DataSize();
	if (dataSize == 0)
	{
		// 만약 dataSize가 0이라면 write한만큼 read도 완료된 상황입니다. 
		_readPos = _writePos = 0;
	}
	else
	{
		// Clean 으로 들어 왔는데 read,write 좌표가 일치 하지 않았다는것은 
		// 데이터와 커서들을 같이 앞으로 땡겨야 한다는 말입니다. 

		// 먼저 데이터를 땡기는 코드입니다.
		//::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
		//_readPos = 0;
		//_writePos = dataSize;
		

		// 여기 까지만 하면 깔끔한 코드가 될텐데 좀 더 성능상 개량할 여지가 있습니다. 
		// TCP통신이다 보니 한 패킷만큼씩 전송되는것이 아니고 일부 데이터가 잘려서 
		// 도착 할 수도 있습니다. 그러다 보면 버퍼에 여유가 있어도 조금 남은 데이터는 일단 
		// 0번 인덱스로 이동시켜줘야 하는 상황이 생깁니다. 
		// 데이터 이동에 복사 비용이 발생하는데 이것을 최대한 줄여주는 방법이 있습니다. 
		// 처음부터 버퍼 사이즈를 아주 널널하게 잡고 이 Clean 함수를 미뤄서 최대한 
		// memcpy 없이 커서만 옮길 수 있도록 유도하는것입니다. 

		// 그래서 이제는 여유 공간이 버퍼 1개 크기 미만이면 데이터를 앞으로 땡깁니다.
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
	// 성공적으로 데이터를 읽은 후 _readPos 커서를 앞으로 땡겨줍니다. 

	// 먼저 예외 처리로 인자인 numOfBytes는 읽은 데이터 크기 입니다. 
	// numOfBytes보다 _writePos, _readPos의 차이 값보다 크다는것은 뭔가 이상한 상황입니다.
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	// OnRead와 비슷한데 성공적으로 데이터를 썼으면 _writePos를 앞으로 땡깁니다. 

	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}
