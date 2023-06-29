#include "pch.h"
#include "DeadLockProfiler.h"

/*----------------------
	DeadLockProfiler
------------------------*/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// 잡고 있는 락이 있었다면 
	if (LLockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 다시 확인한다.
		const int32 prevId = LLockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}
	
	LLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (LLockStack.empty())
		CRASH("MUTIPLE_UNLOCK");

	int32 lockId = _nameToId[name];
	if (LLockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	
	LLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	// 사이클 체크에 필요한 중간 임시 변수들을 초기화
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// 연산이 끝났으면 정리한다
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	// 이미 방문한 상태이면 바로 리턴
	if (_discoveredOrder[here] != -1)
		return;

	// 여기 부터 아직 방문하지 않은 상태

	// 방문하고 있는 정점에 순번을 매기면서 카운팅도 같이 하고 있습니다.
	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회한다. 인접한 정점은 _lockHistory 기록됩니다.

	
	auto findIt = _lockHistory.find(here);
	// _lockHistory에 기록된 정점이 없다면 처음 으로 등록된 락 
	if (findIt == _lockHistory.end())	
	{
		_finished[here] = true;
		return;
	}

	// 여기부터는 다른 락이 걸려있는 상태에서 다시 락이 걸린 경우 
	// 사이클을 판별해야함

	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there 보다 먼저 발견되었다면, there는 here의 후손이다.(순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 순방향이 아니고 Dfs(there)가 아직 종료하지 않았다면, 
		// there는 here의 선조이다.(역방향 간선)
		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _idToName[here], _idToName[there]);

			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}

