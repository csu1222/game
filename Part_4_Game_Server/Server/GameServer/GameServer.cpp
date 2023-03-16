#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;

	ready.store(true, memory_order::memory_order_seq_cst);
}

void Consumer()
{
	while (ready.load(memory_order::memory_order_seq_cst) == false)
		;

	cout << value << endl;
}

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

	// Memory Model (정책)
	// 1) Sequentially Consistent (sep_cst)
	// 2) Acquire_Release (consume, acquire, release, acq_rel)
	// 3) Relaxed (relaxed)

	// 1) seq_cst
	// 가시성 문제 바로 해결! 코드 재배치 바로 해결!
	// 
	// 2) acquire-release
	// 딱 중간!
	// release 명령 이전의 메모리 명령들이 , 해당 명령 이후로 재배치 되는 것을 금지 
	// 그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면 
	// release 이전의 명령들이 -> acquire 하는 순간에 관찰 가능 (가시성 보장)
	 
	// 3) relaxed
	// 너무나도 자유롭다!
	// 코드 재배치도 멋대로 가능! 가시성 해결 NO!
	// 가장 기본 조건 (동일 객체에 대한 동일 수정 순서만 보장)

	// 추가로 인텔, AMD CPU의 경우에는 애당초 순차적 일관성을 보장해서
	// sep_cst를 써도 별다른 차이가 없습니다.그래서 그냥 냅둬도 된다는겁니다.
	// ARM 같은 경우에는 꽤 차이가 있어서 의미가 있다고 합니다.
}