#include "pch.h"
#include "SendBuffer.h"

/*
-------------------------
	SendBuffer 정의부
-------------------------
*/

SendBuffer::SendBuffer(SendBufferChunkRef owner, BYTE* buffer, int32 allocSize)
	: _owner(owner), _buffer(buffer), _allocSize(allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 writeSize)
{
	// 기존 Capacity() 대신 _allocSize
	ASSERT_CRASH(_allocSize >= writeSize);

	// 앞으로 memcpy는 외부에서 해줄것입니다.  
	//::memcpy(_buffer.data(), data, len);

	_writeSize = writeSize;

	// 할당받은 메모리중 실제 사용한 크기인 writeSize로 확정을 지어줍니다. 
	_owner->Close(_writeSize);
}


/*
------------------------------
	SendBufferChunk 정의부
------------------------------
*/
SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
}

void SendBufferChunk::Reset()
{
	_open = false;
	_usedSize = 0;	// 이걸 0으로 밀어주면서 이 버퍼청크의 시작 위치부터 다시 사용할 수 있도록 합니다. 
}

SendBufferRef SendBufferChunk::Open(uint32 allocSize)
{
	// 예외처리를 두가지 하고 있습니다. 할당받고 싶은 크기가 정해뒀던 버퍼 청크의 최대 크기 보다 작은가?
	// 이미 Open을 하고 있는 중인가? 를 체크합니다. 
	ASSERT_CRASH(allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(_open == false);

	// allocSize가 남은 사이즈보다 크다면 Open 실패로 nullptr을 반환
	if (allocSize > FreeSize())
		return nullptr;

	// 여기 까지 왔다면 Open을 할 수 있습니다. 
	_open = true;
	 
	// 반환으로 ObjectPool을 사용해 반환 해주게 됩니다. 
	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(uint32 writeSize)
{
	// open을 하지 않았는데 close 할 수는 없습니다. 
	ASSERT_CRASH(_open == true);
	// 체크후에는 잊지말고 _open을 false로, _usedSize는 writeSize 만큼 증가
	_open = false;
	_usedSize += writeSize;
}


/*
------------------------------
	SendBufferManager 정의부
------------------------------
*/
SendBufferRef SendBufferManager::Open(uint32 size)
{
	// 큼지막한 버퍼덩어리에서 사용할 만큼을 쪼개 가져가겠다

	// Open으로 가져간 영역의 버퍼를 꼭 다 사용할 필요는 없고 나중에 버퍼에 저장할 만큼
	// 저장한 다음 Close를 해주게 될건데 그 Close된 영역이 실제 버퍼에서 사용된 데이터 크기입니다. 

	// 실질적으로 컨텐츠 단에서 사용하는 함수는 이 Open입니다. 

	// 매번 GSendBufferManager 에서 SendBufferChunk를 꺼내사용하기 보다는 
	// TLS에 SendBufferChunkRef를 하나 들고 있게 해서 최대한 락을 덜 걸어도 되도록 해주겠습니다. 
	// CoreTLS 파일들에 LSendBufferChunk 로 추가했습니다. 


	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();	// Pop내부에서 WRITE_LOCK 
		LSendBufferChunk->Reset();
	}
	// LSendBufferChunk 가 이미 Open을 한 상태인지 체크
	ASSERT_CRASH(LSendBufferChunk->IsOpen() == false);

	// 다 썼으면 버리고 새거로 교체
	// 다 썻다는것이 무슨 말이냐 하면 SendBufferChunk를 사용하는 방법이 할당된 메모리의 
	// 앞쪽 주소부터 필요한 만큼 쪼개 사용한다음 한번 쪼갠 만큼의 메모리는 재사용하지 않고 
	// 다음 뒷쪽 주소로 쭉 이어 사용합니다. 그러면서 SendBufferChunk의 용량을 다 사용하면 
	// 기존것은 폐기하고 새것을 Pop해 사용합니다. 그러면 기존것이 폐기된다는것은 어떤 의미인가 하면 
	// 아무도 그 BufferChunk를 참조하지 않게 되면 A_delete 대신 설정해준 PushGlobal이 호출되어 
	// 다시 _sendBufferChunks로 들어오게 됩니다. 

	// 스레드의 로컬 영역에 있는 버퍼 청크의 남은 공간이 Open하려고 하는 size 보다 작다면
	if (LSendBufferChunk->FreeSize() < size)
	{
		// 새로 Pop을 해줍니다. 꺼낸 버퍼청크는 Reset으로 밀어줍니다. 
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	// 여기까지 왔다면 이 스레드의 버퍼 청크는 원래 nullptr 이었으면 Pop,
	// 남은공간이 충분치 않았다면 Pop, 이미 버퍼청크가 있고 남은공간도 충분하다면 그냥 그대로 반환합니다. 

	cout << "FREE : " << LSendBufferChunk->FreeSize() << endl;

	return LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop()
{

	cout << "Pop SendBufferChunk" << endl;
	{
		// 락의 영역을 조절하는 중괄호
		WRITE_LOCK;
		if (_sendBufferChunks.empty() == false)
		{
			// 풀이 비지 않았으면 그 청크를 반환합니다. 
			SendBufferChunkRef sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		} 
	}

	return SendBufferChunkRef(A_new<SendBufferChunk>(), PushGlobal);
	// SendBufferChunkRef를 반환하는 방법이 생소합니다. 평소에는 shared_ptr은 MakeShared를 통해 
	// A_new를 해주면서 PoolAllocator 를 사용하고, A_delete를 통해 또 PoolAllocator::Release를 
	// 통해 메모리 풀로 돌아가고 있었는데 
	// 이것들을 직접 A_new<SendBufferChunk>(), PushGlobal를 사용하게끔 만들어 주고 있는겁니다.
	// SendBufferChunkRef의 참조 카운트가 0이 되었을때 호출되는 PushGlobal 로 인해 메모리를 날리는게 아닌
	// SendBufferManager::_sendBufferChunks 로 돌아오게끔 해줍니다. 
	// 그러니까 계속 재사용을 할것이라는 말입니다. 
	// 이렇게 외부에서 사용할수 있도록 PushGlobal 함수만 static 이 붙은것입니다. 
}

void SendBufferManager::Push(SendBufferChunkRef buffer)
{
	WRITE_LOCK;
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	cout << "PushGlobal SendBufferChunk" << endl;
	// 글로벌 객체인 GSendBufferManager에 Push해줍니다. 이때 다시 참조 카운트 0일때 PushGlobal이 
	// 실행되도록 합니다. 
	GSendBufferManager->Push(SendBufferChunkRef(buffer, PushGlobal));
}
