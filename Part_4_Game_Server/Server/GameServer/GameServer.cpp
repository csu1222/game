#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex  m;
queue<int32> q;
HANDLE handle;

// 참고) CV 는 User-Level Object (커널 오브젝트X)
condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고 
		// 4) 조건변수 통해 다른 쓰레드에게 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // wait 중인 쓰레드가 있으면 딱 1개를 깨운다

		this_thread::yield();
	}
}

void Consumer()
{
	while (true)
	{

		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; });
		// 1) Lock을 잡고 
		// 2) 조건 확인
		// - 만족 O -> 빠져 나와서 이어서 코드를 진행 
		// - 만족 X -> Lock을 풀어주고 대기 상태로 전환 

		// 그런데 notify_one을 했으면 항상 조건식을 만족하는거 아닐까>? 
		// Spurious Wakeup (직영하면 가짜 기상?)
		// notify_one을 할 때 lock을 잡고 있는 것이 아니기 때문

		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트 
	// Usage Count		몇개나 참조하고 있는가
	// Signal (파란불) / Non-Signal (빨간불)  << bool 
	// Auto / Manual  << bool 
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

}
