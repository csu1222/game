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
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

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
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;
};