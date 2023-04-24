#pragma once


// 특정 크기 범위에 들어가는 데이터를 관리하는 메모리 풀 

// 데이터 앞에 붙일 메모리 헤더 데이터의 크기는 얼마인지, 힙에 할당된 다음 메모리
// 주소는 어디인지 등의 정보를 담는 헤더'
/*-----------------
	MemoryHeader
------------------*/
struct MemoryHeader
{
	// [MemoryHeader][Data]
	MemoryHeader(int32 size) : allocSize(size) { }

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header) MemoryHeader(size);	// placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;

	// TODO : 필요한 정보 추가 
};


/*----------------
	Memoty Pool
------------------*/
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();
private:
	int32 _allocSize = 0;
	atomic<int32> _allocCount = 0;

	USE_LOCK;
	queue<MemoryHeader*> _queue;
};

