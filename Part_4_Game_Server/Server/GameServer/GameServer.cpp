#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

// 소수 구하기 

/*
	1과 자기 자신으로만 나누어 떨어지는 1보다 큰 양의 정수라고 합니다. 

*/

atomic<int> result = 0;

bool isPrime(int number)
{
	if (number <= 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i < number; i++)
	{
		if (!(number % i))
			return false;
	}

	return true;
}

void ThreadCount(int start, int end)
{
	for (int i = start; i <= end; i++)
	{
		if (!(isPrime(i)))
			continue;

		result++;
	}
}

int main()
{
	const int MAX_NUMBER = 100'0000;

	// 1 ~ MAX_NUMBER까지의 소수 개수 

	int coreCount = thread::hardware_concurrency();
	int jobCount = (MAX_NUMBER / coreCount) + 1;

	vector<thread> threads = vector<thread>(coreCount);

	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));
		threads[i] = thread([start, end]()
			{
				ThreadCount(start, end);
			});
	}

	for (thread& t : threads)
		t.join();

	cout << result << endl;
}