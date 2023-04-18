#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"

class Player
{
public:
	Player() { }
	virtual ~Player() { }
};

class Knight : public Player
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	//static void operator delete(void* ptr)
	//{
	//	cout << "Knight delete!" << endl;
	//	::free(ptr);
	//}

	//static void* operator new(size_t size)
	//{
	//	cout << "Knight new! " << size << endl;
	//	void* ptr = ::malloc(size);
	//	return ptr;
	//}

	int32 _hp = 100;
	int32 _mp = 10;
};

// new operator overloading (Global)
void* operator new(size_t size)
{
	cout << "new! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete(void* ptr)
{
	cout << "delete!" << endl;
	::free(ptr);
}

void* operator new[](size_t size)
{
	cout << "new[]! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete[](void* ptr)
{
	cout << "delete![]" << endl;
	::free(ptr);
}

int main()
{
	Knight* k1 = A_new<Knight>(100);


	A_delete(k1);
	
	k1->_hp = 100;

}