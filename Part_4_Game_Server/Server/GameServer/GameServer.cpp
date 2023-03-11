#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

int64 Calculator()
{
	int32 sum = 0;

	for (int32 i = 0; i < 100'000; i++)
		sum += i;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task < int64(void)> && task)
{
	task();
}

int main()
{
	// 동기(Synchronus) 실행
	// int64 sum = Calculator();


	// std::future
	{
		std::future<int64> future = std::async(std::launch::async, Calculator);

		// TODO 
		// future 객체의 작업이 끝났는지 알아보기 
		std::future_status status = future.wait_for(1ms);

		if (status == future_status::ready)
		{

		}

		int64 sum = future.get();

	}

		// 추가 디테일 : future가 들고 있을 함수가 다른 객체의 메소드라면?
	{
		class Knight
		{
		public:
			int64 GetHP() { return 100; }
		};

		Knight knight;
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHP, knight);
	}

	// std::promise
	{
		// 미래(std::futur)에 결과물을 반환해줄꺼라 약속(std::promise) 해줘 (계약서?)
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// packaged_task
	{
		std::packaged_task<int64(void)> task(Calculator);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// 결론)
	// mutex, condition_variable 까지 가지 않고 단순한 애들을 처리할 수 있는 방법
	// 특히나, 한번 발생하는 이벤트에 유용하다!
	// 닭잡는데 소잡는 칼을 쓸 필요 없다!
	// 1) async
	// 원하는 함수를 비동기적으로 실행
	// 2) promise
	// 결과물을 promise를 통해 future로 받아줌 
	// 3) packaged_task
	// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌 
}
