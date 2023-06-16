#pragma once

/*----------------
	Base Allocator
------------------*/

class BaseAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};

/*----------------
	Stomp Allocator
------------------*/

class StompAllocator
{
	enum { PAGE_SIZE = 0x1000, };
public:
	static void* Alloc(int32 size);
	static void		Release(void* ptr);
};

/*----------------
	Pool Allocator
------------------*/

class PoolAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};


/*----------------
	STL Allocator
------------------*/

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() { }

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(PoolAllocator::Alloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::Release(ptr);
	}
};

/*
IocpCore 강의중 Listener 클래스를만들면서 Vector라는 우리만의 StlAllocator를 
사용하는 컨테이너를 사용하면서 컴파일러가 에러를 잡아 주었습니다. 
이전에는 allocate 함수와 deallocate 함수에서 A_alloc, A_delete 를 사용하고 있었는데 
PoolAllocator 를 만들면서 더이상 A_ 시리즈는 사용하지 않았고 
Memory::Alloc, Memory::Release 를 사용하게 만들었고 또 내부에서 
Types.h 에서 관리하는 define STOMP 가 유효하냐 안하냐에 따라 
StompAllocator 를 사용할지 PoolAllocator를 사용할지 골라주게 했습니다. 
*/