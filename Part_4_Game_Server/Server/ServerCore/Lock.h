#pragma once
#include "Types.h"

/*--------------------------------
			RW SpinLock
----------------------------------*/

/*--------------------------------------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)
---------------------------------------------*/


// W -> R (O)
// R -> W (X)
class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,		// 최대로 기다려줄 틱
		MAX_SPIN_COUNT = 5000,				// 최대로 회전할 수 
		WRITE_THREAD_MASK = 0xFFFF'0000,	// write flag 를 비트플레그로
		// 관리 할것 이기 때문에 정확하게 상위 16비트를 뽑아오기 위한 마스크
		READ_COUNT_MASK = 0x0000'FFFF,		// 반대로 READ COUNT를 위한 마스크
		EMPTY_FLAG = 0x0000'0000,
		/*
		비트 플레그가 익숙하지 않을 수 있는데 위에서 F로 되어 있는 부분은 
		2진법으로 보면 1로 되어 있고 그 부분을 OR 연산으로 긁어 오기 위한 
		마스크
		*/
	};
public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};


/*---------------------------------------
           Lock Guard
-----------------------------------------*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnlock(); }
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnlock(); }
private:
	Lock& _lock;
};