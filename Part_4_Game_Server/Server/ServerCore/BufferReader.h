#pragma once

/*
---------------------------
	BufferReader ����� 
---------------------------
*/

// BufferReader�� ������ ������ �����ּҿ� �� ������ ũ�⸦ �޾� �����ϴ� ����
class BufferReader
{
public:
	BufferReader();
	BufferReader(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferReader();

	BYTE*				Buffer() { return _buffer; }
	uint32				Size() { return _size; }
	uint32				ReadSize() { return _pos; }
	uint32				FreeSize() { return _size - _pos; }
	
	// Peek�� �� �� ����ϱ� ���� 
	template<typename T>
	bool				Peek(T* dest) { return Peek(dest, sizeof(T)); }
	// Peek�� Ŀ���� �ű����� �ʰ� ��� ������ ������ 
	bool				Peek(void* dest, uint32 len);

	// ���뵵 Ȯ���ϰ� Ŀ���� ������ �ű�ϴ�. 
	template<typename T>
	bool				Read(T* dest) { return Read(dest, sizeof(T)); }
	bool				Read(void* dest, uint32 len);

	// ���� ����ϴ� cout << �� << �� ������ �����ε� ����̾����ϴ�.
	template<typename T>
	BufferReader&		operator>>(OUT T& dest);

private:
	BYTE*				_buffer = nullptr;	// ������ �����ּ� 
	uint32				_size = 0;			// ������ ������
	uint32				_pos = 0;			// ���� ������ �о��°�
};

template<typename T>
inline BufferReader& BufferReader::operator>>(OUT T& dest)
{
	// >> �����ڴ� BufferReader���� ���ݱ��� ���� ��ġ���� ���빰������ 
	// dest�� ��� �شٴ� �ǹ� 

	// _buffer[_pos]�� ��ġ�� �����͸� TŸ������ ĳ�����ؼ� dest�� ����ϴ�. 
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);
	// dest�� ��� �����͸�ŭ�� �о��ٰ� ġ�� Ŀ���� �ű�ϴ�. 
	_pos += sizeof(T);
	return *this;
}