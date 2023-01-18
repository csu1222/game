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
vector<bool> visited;

void CreateGraph ()
{
	vertices.resize(6);
	adjacent = vector<vector<int>>(6);
	// 내용을 넣어주는 방법으로 push_back 이나 {} 초기화를 사용합니다.

	//// 인접 리스트
	//adjacent[0].push_back(1);
	//adjacent[0].push_back(3);
	//adjacent[1].push_back(0);
	//adjacent[1].push_back(2);
	//adjacent[1].push_back(3);
	//adjacent[3].push_back(4);
	//adjacent[5].push_back(4);

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

void Dfs(int here)
{
	// 방문!
	visited[here] = true;

	cout << "Visited : " << here << endl;
	
	// 인접 리스트 버전
	// 모든 인접 정점을 순회한다 
	//for (int i = 0; i < adjacent[here].size(); i++)
	//{
	//	// 방문할 방 there
	//	int there = adjacent[here][i];

	//	// 방문했는지 체크 후 방문하지 않았으면 방문 
	//	if (visited[there] == false)
	//		Dfs(there);
	//}

	// 인접 행렬 버전
	for (int there = 0; there < 6; there++)
	{
		if (adjacent[here][there] == 0)
			continue;

		if (visited[there] == false)
			Dfs(there);
	}
}

void DfsAll()
{

	visited = vector<bool>(6, false);

	for (int i = 0; i < 6; i++)
		if (visited[i] == false)
			Dfs(i);
}

int main()
{
	CreateGraph();

	// visited = vector<bool>(6, false);

	DfsAll();
}