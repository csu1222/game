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
		ACQUIRE_TIMEOUT_TICK = 10000,		// �ִ�� ��ٷ��� ƽ
		MAX_SPIN_COUNT = 5000,				// �ִ�� ȸ���� �� 
		WRITE_THREAD_MASK = 0xFFFF'0000,	// write flag �� ��Ʈ�÷��׷�
		// ���� �Ұ� �̱� ������ ��Ȯ�ϰ� ���� 16��Ʈ�� �̾ƿ��� ���� ����ũ
		READ_COUNT_MASK = 0x0000'FFFF,		// �ݴ�� READ COUNT�� ���� ����ũ
		EMPTY_FLAG = 0x0000'0000,
		/*
		��Ʈ �÷��װ� �ͼ����� ���� �� �ִµ� ������ F�� �Ǿ� �ִ� �κ��� 
		2�������� ���� 1�� �Ǿ� �ְ� �� �κ��� OR �������� �ܾ� ���� ���� 
		����ũ
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