#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// DFS (Depth First Search) 깊이 우선 탐색
// BFS (Breadth First Search) 너비 우선 탐색

struct Vertex
{
	// int _data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;
vector<bool> discovered;

void CreateGraph ()
{
	vertices.resize(6);
	adjacent = vector<vector<int>>(6);
	// 내용을 넣어주는 방법으로 push_back 이나 {} 초기화를 사용합니다.

	// 인접 리스트
	/*adjacent[0].push_back(1);
	adjacent[0].push_back(3);
	adjacent[1].push_back(0);
	adjacent[1].push_back(2);
	adjacent[1].push_back(3);
	adjacent[3].push_back(4);
	adjacent[5].push_back(4);*/

	// 인접 행렬
	adjacent = vector<vector<int>>
	{
		{ 0, 1, 0, 1, 0, 0},
		{ 1, 0, 1, 1, 0, 0},
		{ 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 1, 0},
		{ 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 1, 0},
	};

}

void Bfs(int here)
{
	// 나를 발견한 부모를 추적 
	vector<int> parent = vector<int>(6, -1);
	// 시작점으로 부터의 거리
	vector<int> distance = vector<int>(6, -1);

	// 발견한 목록을 기록할 큐 
	queue<int> q;
	q.push(here);
	discovered[here] = true;

	parent[here] = here;
	distance[here] = 0;

	while (q.empty() == false)
	{
		here = q.front();
		q.pop();

		cout << "Visited : " << here << endl;

		for (int there = 0; there < 6; there++)
		{
			if (adjacent[here][there] == 0)
				continue;
			if (discovered[there])
				continue;

			q.push(there);
			discovered[there] = true;
			parent[there] = here;
			distance[there] = distance[here] + 1;
		}
	}
}

void BfsAll()
{
	for (int i = 0; i < 6; i++)
	{
		if (discovered[i] == false)
			Bfs(i);
	}
}

int main()
{
	CreateGraph();

	discovered = vector<bool>(6, false);

	BfsAll();
}