#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>


// Atomic : Atom(원자)  All-or-Nothing

// DB 에서의 Atomic 예시
// 
// - A 라는 유저 인벤에서 집행검을 빼고 
// - B 라는 유저 인벤에 집행검을 추가 
// -- 위의 두줄을 아토믹하게 실행해야 합니다. 

atomic<int32> sum = 0;

void Add()
{
	for (int i = 0; i < 100'0000; i++)
	{
		//sum++;
		sum.fetch_add(1);
	}
}

void Sub()
{
	for (int i = 0; i < 100'0000;  i++)
	{
		//sum--;
		sum.fetch_add(-1);
	}
}

int main()
{
	Add();
	Sub();

	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();

	cout << sum << endl;
}
