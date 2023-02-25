#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// Lock 기초 

vector<int32> v;

// Mutual Exclusive (상호배타적)
mutex m;

// RAII(Resource Acquisition Is Initialization)
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}
	~LockGuard()
	{
		_mutex->unlock();
	}

private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; i++)
	{
		// 자물쇠 잠그기
		//std::lock_guard<std::mutex> lockGuard(m);
		std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
		uniqueLock.lock();
		//m.lock();

		v.push_back(i);

		if (i == 5000)
		{
			// m.unlock();
			break;
		}

		// 자물쇠 풀기
		//m.unlock();
	}
}

int main()
{
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}
