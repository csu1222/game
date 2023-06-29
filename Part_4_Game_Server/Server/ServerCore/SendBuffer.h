#pragma once

/*
-------------------------
	SendBuffer 선언부 
-------------------------
*/

// 이전에 SendEvent에 임시로 만들어 놨던 버퍼를 클래스화 해서 만들어 보겠습니다. 
class SendBuffer : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.size()); }

	void CopyData(void* data, int32 len);

private:
	Vector<BYTE>	_buffer;
	// 매번 버퍼의 모든 용량을 사용하지는 않을것이고 그때 그때 버퍼의 일부분만 사용할 수 있습니다. 
	int32			_writeSize = 0;
};

