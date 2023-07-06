#pragma once

/*
---------------------------
	BufferWriter �����
---------------------------
*/

// BufferReader �� ����� �κ��� �����ϴ�. 
class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE*				Buffer() { return _buffer; }
	uint32				Size() { return _size; }
	uint32				WriteSize() { return _pos; }
	uint32				FreeSize() { return _size - _pos; }

	// ���ۿ� �ҽ�(src)�� �ۼ��մϴ�. 
	template<typename T>
	bool				Write(T* src) { return Read(src, sizeof(T)); }
	bool				Write(void* src, uint32 len);

	// ���۸� �ۼ��Ҷ� ����� ���� �������� �ۼ��Ҷ��� �ִµ� �׶� ����� �Լ�
	template<typename T>
	T*					Reserve();

	// �����͸� �о� �ִ´ٴ� ���� << ������ 
	// ���ø��� ����ϸ� ���� ���� �����̾����ϴ�. 
	template<typename T>
	BufferWriter& operator<<(T&& src);

private:
	// ��� ������ BufferReader�� �Ȱ����ϴ�. 
	BYTE*				_buffer = nullptr;	// ������ �����ּ� 
	uint32				_size = 0;			// ������ ������
	uint32				_pos = 0;			// ������ ���°�
};

template<typename T>
T* BufferWriter::Reserve()
{
	// ������ TŸ�� ��ŭ�� ������ ���ٸ� ����
	if (FreeSize() < sizeof(T))
		return nullptr;

	// ������ �ִٸ� ������ġ�� �����͸� ret �� ��� ��ȯ �Ұ�
	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	// �ٸ� T ��ŭ Ŀ���� ����ٰ�
	_pos += sizeof(T);

	return ret;
}


template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	using DataType = std::remove_reference_t<T>;
	*reinterpret_cast<DataType*>(&_buffer[_pos]) = std::forward< DataType>(src);
	_pos += sizeof(T);
	return *this;
}
