﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And_Swap)

		bool expected = false;
		bool desired = true;

		// CAS 의사 코드 
		/*if (_locked == expected)
		{
			expected = _locked;
			_locked = desired;
			return true;
		}
		else
		{
			expected = _locked;
			return false;
		}*/

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}

	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int i = 0; i < 100000; i++)
	{
		lock_guard<SpinLock> gaurd(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int i = 0; i < 100000; i++)
	{
		lock_guard<SpinLock> gaurd(spinLock);
		sum--;
	}
}

int main()
{


	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}
