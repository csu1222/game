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
IocpCore ������ Listener Ŭ����������鼭 Vector��� �츮���� StlAllocator�� 
����ϴ� �����̳ʸ� ����ϸ鼭 �����Ϸ��� ������ ��� �־����ϴ�. 
�������� allocate �Լ��� deallocate �Լ����� A_alloc, A_delete �� ����ϰ� �־��µ� 
PoolAllocator �� ����鼭 ���̻� A_ �ø���� ������� �ʾҰ� 
Memory::Alloc, Memory::Release �� ����ϰ� ������� �� ���ο��� 
Types.h ���� �����ϴ� define STOMP �� ��ȿ�ϳ� ���ϳĿ� ���� 
StompAllocator �� ������� PoolAllocator�� ������� ����ְ� �߽��ϴ�. 
*/