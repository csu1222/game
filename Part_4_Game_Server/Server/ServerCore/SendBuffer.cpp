#include "pch.h"
#include "SendBuffer.h"

/*
-------------------------
	SendBuffer ���Ǻ�
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
	// ���� Capacity() ��� _allocSize
	ASSERT_CRASH(_allocSize >= writeSize);

	// ������ memcpy�� �ܺο��� ���ٰ��Դϴ�.  
	//::memcpy(_buffer.data(), data, len);

	_writeSize = writeSize;

	// �Ҵ���� �޸��� ���� ����� ũ���� writeSize�� Ȯ���� �����ݴϴ�. 
	_owner->Close(_writeSize);
}


/*
------------------------------
	SendBufferChunk ���Ǻ�
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
	_usedSize = 0;	// �̰� 0���� �о��ָ鼭 �� ����ûũ�� ���� ��ġ���� �ٽ� ����� �� �ֵ��� �մϴ�. 
}

SendBufferRef SendBufferChunk::Open(uint32 allocSize)
{
	// ����ó���� �ΰ��� �ϰ� �ֽ��ϴ�. �Ҵ�ް� ���� ũ�Ⱑ ���ص״� ���� ûũ�� �ִ� ũ�� ���� ������?
	// �̹� Open�� �ϰ� �ִ� ���ΰ�? �� üũ�մϴ�. 
	ASSERT_CRASH(allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(_open == false);

	// allocSize�� ���� ������� ũ�ٸ� Open ���з� nullptr�� ��ȯ
	if (allocSize > FreeSize())
		return nullptr;

	// ���� ���� �Դٸ� Open�� �� �� �ֽ��ϴ�. 
	_open = true;
	 
	// ��ȯ���� ObjectPool�� ����� ��ȯ ���ְ� �˴ϴ�. 
	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(uint32 writeSize)
{
	// open�� ���� �ʾҴµ� close �� ���� �����ϴ�. 
	ASSERT_CRASH(_open == true);
	// üũ�Ŀ��� �������� _open�� false��, _usedSize�� writeSize ��ŭ ����
	_open = false;
	_usedSize += writeSize;
}


/*
------------------------------
	SendBufferManager ���Ǻ�
------------------------------
*/
SendBufferRef SendBufferManager::Open(uint32 size)
{
	// ŭ������ ���۵������ ����� ��ŭ�� �ɰ� �������ڴ�

	// Open���� ������ ������ ���۸� �� �� ����� �ʿ�� ���� ���߿� ���ۿ� ������ ��ŭ
	// ������ ���� Close�� ���ְ� �ɰǵ� �� Close�� ������ ���� ���ۿ��� ���� ������ ũ���Դϴ�. 

	// ���������� ������ �ܿ��� ����ϴ� �Լ��� �� Open�Դϴ�. 

	// �Ź� GSendBufferManager ���� SendBufferChunk�� ��������ϱ� ���ٴ� 
	// TLS�� SendBufferChunkRef�� �ϳ� ��� �ְ� �ؼ� �ִ��� ���� �� �ɾ �ǵ��� ���ְڽ��ϴ�. 
	// CoreTLS ���ϵ鿡 LSendBufferChunk �� �߰��߽��ϴ�. 


	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();	// Pop���ο��� WRITE_LOCK 
		LSendBufferChunk->Reset();
	}
	// LSendBufferChunk �� �̹� Open�� �� �������� üũ
	ASSERT_CRASH(LSendBufferChunk->IsOpen() == false);

	// �� ������ ������ ���ŷ� ��ü
	// �� ���ٴ°��� ���� ���̳� �ϸ� SendBufferChunk�� ����ϴ� ����� �Ҵ�� �޸��� 
	// ���� �ּҺ��� �ʿ��� ��ŭ �ɰ� ����Ѵ��� �ѹ� �ɰ� ��ŭ�� �޸𸮴� �������� �ʰ� 
	// ���� ���� �ּҷ� �� �̾� ����մϴ�. �׷��鼭 SendBufferChunk�� �뷮�� �� ����ϸ� 
	// �������� ����ϰ� ������ Pop�� ����մϴ�. �׷��� �������� ���ȴٴ°��� � �ǹ��ΰ� �ϸ� 
	// �ƹ��� �� BufferChunk�� �������� �ʰ� �Ǹ� A_delete ��� �������� PushGlobal�� ȣ��Ǿ� 
	// �ٽ� _sendBufferChunks�� ������ �˴ϴ�. 

	// �������� ���� ������ �ִ� ���� ûũ�� ���� ������ Open�Ϸ��� �ϴ� size ���� �۴ٸ�
	if (LSendBufferChunk->FreeSize() < size)
	{
		// ���� Pop�� ���ݴϴ�. ���� ����ûũ�� Reset���� �о��ݴϴ�. 
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	// ������� �Դٸ� �� �������� ���� ûũ�� ���� nullptr �̾����� Pop,
	// ���������� ���ġ �ʾҴٸ� Pop, �̹� ����ûũ�� �ְ� ���������� ����ϴٸ� �׳� �״�� ��ȯ�մϴ�. 

	cout << "FREE : " << LSendBufferChunk->FreeSize() << endl;

	return LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop()
{

	cout << "Pop SendBufferChunk" << endl;
	{
		// ���� ������ �����ϴ� �߰�ȣ
		WRITE_LOCK;
		if (_sendBufferChunks.empty() == false)
		{
			// Ǯ�� ���� �ʾ����� �� ûũ�� ��ȯ�մϴ�. 
			SendBufferChunkRef sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		} 
	}

	return SendBufferChunkRef(A_new<SendBufferChunk>(), PushGlobal);
	// SendBufferChunkRef�� ��ȯ�ϴ� ����� �����մϴ�. ��ҿ��� shared_ptr�� MakeShared�� ���� 
	// A_new�� ���ָ鼭 PoolAllocator �� ����ϰ�, A_delete�� ���� �� PoolAllocator::Release�� 
	// ���� �޸� Ǯ�� ���ư��� �־��µ� 
	// �̰͵��� ���� A_new<SendBufferChunk>(), PushGlobal�� ����ϰԲ� ����� �ְ� �ִ°̴ϴ�.
	// SendBufferChunkRef�� ���� ī��Ʈ�� 0�� �Ǿ����� ȣ��Ǵ� PushGlobal �� ���� �޸𸮸� �����°� �ƴ�
	// SendBufferManager::_sendBufferChunks �� ���ƿ��Բ� ���ݴϴ�. 
	// �׷��ϱ� ��� ������ �Ұ��̶�� ���Դϴ�. 
	// �̷��� �ܺο��� ����Ҽ� �ֵ��� PushGlobal �Լ��� static �� �������Դϴ�. 
}

void SendBufferManager::Push(SendBufferChunkRef buffer)
{
	WRITE_LOCK;
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	cout << "PushGlobal SendBufferChunk" << endl;
	// �۷ι� ��ü�� GSendBufferManager�� Push���ݴϴ�. �̶� �ٽ� ���� ī��Ʈ 0�϶� PushGlobal�� 
	// ����ǵ��� �մϴ�. 
	GSendBufferManager->Push(SendBufferChunkRef(buffer, PushGlobal));
}
