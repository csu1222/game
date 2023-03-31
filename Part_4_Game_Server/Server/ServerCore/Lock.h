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