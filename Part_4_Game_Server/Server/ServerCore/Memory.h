#pragma once
#include "Allocator.h"
#include "CoreMacro.h"

class MemoryPool;

/*-------------
	Momory
--------------*/
class Memory
{
	enum
	{
		// ~1024 까지는 32단위, ~2048Rkwl 128단위, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096,
	};

public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*>	_pools;
	
	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];

};

template<typename Type, typename... Args>
Type* A_new(Args&&... args)
{
	Type* memory =	static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));
	
	// placement new	할당된 메모리에 생성자를 호출해주는 문법 
	// new(객체를 만들어줄 공간) 객체의 템플릿(std::forward<Args>(인자들)...);
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

template<typename Type>
void A_delete(Type* obj)
{
	obj->~Type();
	PoolAllocator::Release(obj);
}

template<typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ A_new<Type>(std::forward<Args>(args)...), A_delete<Type> };
}

