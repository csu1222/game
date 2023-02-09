#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>

// 단원 주제 : 최소 스패닝 트리 Minimum Spanning Tree
// 오늘의 주제 : 상호 배타적 집합 Disjoint Set

// 다른 이름으로 
// 유니온-파인드 Union-Find (합치기-찾기)


// 예시 
// Lineage + Battleground (혼종!)
// 사양
// 혈맹전 + 서바이벌
// 1인 팀 1000명까지 (팀ID 0 ~ 999)
// 동맹 가능 (1번팀 + 2번팀 = 새로운 팀) 

void LineageBattleground()
{
	struct User
	{
		int teamId;
		// TODO
	};

	// TODO : UserManager
	vector<User> users;
	for (int i = 0; i < 1000; i++)
	{
		users.push_back(User{ i });
	}

	// 팀 동맹 
	// user 1번 + user 5번 동맹
	users[5].teamId = users[1].teamId;

	// 초반 동맹은 인원이 적으니 단순해 보임 

	// 하지만 후반에 결국 동맹이 커진다면?

	// [1][2][3][4] .... [999]
	// [1][1][1][1]...[2][2][2][2][2] ... [999]
	
	// 세력대 세력의 합병은?
	for (User& user : users)
	{
		if (user.teamId == 1)
			user.teamId = 2;
	}

	// 찾기 연산	O(1)
	// 합치기 연산  O(N)
}

class NaiveDisjointSet
{
public:
	NaiveDisjointSet(int n) : _parent(n)
	{
		for (int i = 0; i < n; i++)
			_parent[i] = i;
	}

	// 니 대장이 누구니?
	int Find(int u)
	{
		if (u == _parent[u])
			return u;

		return Find(_parent[u]);
	}

	// u와 v를 합친다 (일단 u가 v 밑으로)
	void Merge(int u, int v)
	{
		// 일단 대장을 찾는다 
		u = Find(u);
		v = Find(v);

		if (u == v)
			return;

		_parent[u] = v;
	}

private:
	vector<int> _parent;
};

// Union-By-Rank 랭크에 의한 합치기 최적화 
// 높이가 낮은 트리를 높이가 높은 트리 밑으로 병합
class DisjointSet
{
public:
	DisjointSet(int n) : _parent(n), _rank(n, 1)
	{
		for (int i = 0; i < n; i++)
			_parent[i] = i;
	}

	// 니 대장이 누구니?
	int Find(int u)
	{
		if (u == _parent[u])
			return u;

		// _parent[u] = Find(_parent[u]);
		// return _parent[u];
		
		return _parent[u] = Find(_parent[u]);
	}

	// u와 v를 합친다 
	void Merge(int u, int v)
	{
		// 일단 대장을 찾는다 
		u = Find(u);
		v = Find(v);

		if (u == v)
			return;
		
		if (_rank[u] > _rank[v])
			swap(u, v);

		// _rank[u] <= _rank[v] 가 보장됨

		_parent[u] = v;

		if (_rank[u] == _rank[v])
			_rank[v]++;
	}

private:
	vector<int> _parent;
	vector<int> _rank;
};

int main()
{
	DisjointSet teams(1000);

	teams.Merge(10, 1);
	int teamId = teams.Find(10);
	int teamId2 = teams.Find(1);

	teams.Merge(3, 5);
	int teamId3 = teams.Find(3);
	int teamId4 = teams.Find(5);
	
	teams.Merge(1, 3);
	int teamId5 = teams.Find(10);
	int teamId6 = teams.Find(3);
}