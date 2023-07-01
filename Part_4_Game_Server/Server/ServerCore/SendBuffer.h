#pragma once

/*
-------------------------
	SendBuffer 선언부 
-------------------------
*/

// 전방선언 
class SendBufferChunk;
 
class SendBuffer
{
public:
	// 생성자가 받는 인자가 수정되었습니다. 
	SendBuffer(SendBufferChunkRef owner, BYTE* buffer, int32 allocSize);
	~SendBuffer();

	// _buffer가 더이상 Vector 가 아닌 포인터의 형태이기 때문에 그대로 반환
	BYTE* Buffer() { return _buffer; }
	int32 WriteSize() { return _writeSize; }
	void Close(uint32 writeSize);

	// Capacity(), CopyData() 는 더이상 사용하지 않습니다. 
	//int32 Capacity() { return static_cast<int32>(_buffer.size()); }
	//void CopyData(void* data, int32 len);

private:
	// SendBufferChunk를 도입하면서 SendBuffer 자체에서는 _buffer를 Vector로 
	// 들고 있지 않고 시작주소인 BYTE* 와 얼만큼 할당받았는지 allocSize로 범위를 
	// 가지고 있을것입니다. 
	// 또 SendBufferChunk로 부터 가져온 버퍼를 사용하는 중에 먼저 SendBufferChunk가
	// 삭제 되면 안되기 때문에 자신의 owner를 참조 하고 있도록 하겠습니다. 
	BYTE*				_buffer;
	uint32				_allocSize = 0;
	uint32				_writeSize = 0;
	SendBufferChunkRef	_owner;
};


/*
------------------------------
	SendBufferChunk 선언부
------------------------------
*/

class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	// SendBufferChunk는 각 스레드가 SendBufferManager로 부터 꺼낸후 
	// TLS에서 사용할것이기 때문에 싱글스레드에서의 동작을 기준으로 만들 수 있습니다. 

	// 사용할 청크의 사이즈를 enum으로 정해둡니다. 나중에 수정하기 쉽도록 
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void			Reset();
	SendBufferRef	Open(uint32 allocSize);	// 할당할 공간 
	void			Close(uint32 writeSize);			// 실질적으로 사용한 공간 

	bool			IsOpen() { return _open; }	// Open후 Close를 했는지 안했는지 
	BYTE*			Buffer() { return &_buffer[_usedSize]; }	// 현재까지 꺼내간 버퍼이후의 주소 
	uint32			FreeSize() { return static_cast<uint32>(_buffer.size() - _usedSize); }
private:
	// Container.h 에 새로 Array를 추가 
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>	_buffer = {};
	bool								_open = false;
	uint32								_usedSize = 0; // SEND_BUFFER_CHUNK_SIZE 중 지금까지 사용한 크기
};

/*
------------------------------
	SendBufferManager 선언부
------------------------------
*/

// SendBufferManager는 글로벌하게 하나만 만들어 사용할것입니다. 
// CoreGlobal.h 에 추가합니다. 
class SendBufferManager
{
public:
	// 큼지막한 버퍼덩어리에서 사용할 만큼을 쪼개 가져가겠다
	SendBufferRef				Open(uint32 size);

private:
	// SendBufferChunk를 풀에서 꺼내는 함수
	SendBufferChunkRef			Pop();
	// SendBufferChunk를 풀에 반납을 하는 함수
	void						Push(SendBufferChunkRef buffer);

	static void					PushGlobal(SendBufferChunk* buffer);

private:
	// 전역으로 사용할것이다 보니 Lock을 사용합니다. 
	USE_LOCK;
	// Types.h 에 SendBufferChunkRef 정의를 추가 합니다. 
	Vector<SendBufferChunkRef>	_sendBufferChunks;
};