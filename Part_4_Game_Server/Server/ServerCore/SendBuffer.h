#pragma once

/*
-------------------------
	SendBuffer ����� 
-------------------------
*/

// ���漱�� 
class SendBufferChunk;
 
class SendBuffer
{
public:
	SendBuffer(SendBufferChunkRef owner, BYTE* buffer, uint32 allocSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer; }
	// BufferWriter ������ �� _allocSize�� �ʿ��ؼ� �Լ��� ��������ϴ�. 
	uint32 AllocSize() { return _allocSize; }
	uint32 WriteSize() { return _writeSize; }
	void Close(uint32 writeSize);


private: 
	BYTE*				_buffer;
	uint32				_allocSize = 0;
	uint32				_writeSize = 0;
	SendBufferChunkRef	_owner;
};


/*
------------------------------
	SendBufferChunk �����
------------------------------
*/

class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	// SendBufferChunk�� �� �����尡 SendBufferManager�� ���� ������ 
	// TLS���� ����Ұ��̱� ������ �̱۽����忡���� ������ �������� ���� �� �ֽ��ϴ�. 

	// ����� ûũ�� ����� enum���� ���صӴϴ�. ���߿� �����ϱ� ������ 
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void			Reset();
	SendBufferRef	Open(uint32 allocSize);	// �Ҵ��� ���� 
	void			Close(uint32 writeSize);			// ���������� ����� ���� 

	bool			IsOpen() { return _open; }	// Open�� Close�� �ߴ��� ���ߴ��� 
	BYTE*			Buffer() { return &_buffer[_usedSize]; }	// ������� ������ ���������� �ּ� 
	uint32			FreeSize() { return static_cast<uint32>(_buffer.size() - _usedSize); }
private:
	// Container.h �� ���� Array�� �߰� 
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>	_buffer = {};
	bool								_open = false;
	uint32								_usedSize = 0; // SEND_BUFFER_CHUNK_SIZE �� ���ݱ��� ����� ũ��
};

/*
------------------------------
	SendBufferManager �����
------------------------------
*/

// SendBufferManager�� �۷ι��ϰ� �ϳ��� ����� ����Ұ��Դϴ�. 
// CoreGlobal.h �� �߰��մϴ�. 
class SendBufferManager
{
public:
	// ŭ������ ���۵������ ����� ��ŭ�� �ɰ� �������ڴ�
	SendBufferRef				Open(uint32 size);

private:
	// SendBufferChunk�� Ǯ���� ������ �Լ�
	SendBufferChunkRef			Pop();
	// SendBufferChunk�� Ǯ�� �ݳ��� �ϴ� �Լ�
	void						Push(SendBufferChunkRef buffer);

	static void					PushGlobal(SendBufferChunk* buffer);

private:
	// �������� ����Ұ��̴� ���� Lock�� ����մϴ�. 
	USE_LOCK;
	// Types.h �� SendBufferChunkRef ���Ǹ� �߰� �մϴ�. 
	Vector<SendBufferChunkRef>	_sendBufferChunks;
};