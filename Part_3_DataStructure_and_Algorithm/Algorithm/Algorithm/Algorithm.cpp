#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

struct Vertex
{
	// int _data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;	// 인접 행렬

void CreateGraph()
{
	vertices.resize(6);
	adjacent = vector<vector<int>>(6, vector<int>(6, -1));

	// -1, -1, -1, -1, -1, -1
	// -1, -1, -1, -1, -1, -1
	// -1, -1, -1, -1, -1, -1
	// -1, -1, -1, -1, -1, -1
	// -1, -1, -1, -1, -1, -1
	// -1, -1, -1, -1, -1, -1

	//adjacent[0][1] = 15;
	//adjacent[0][3] = 35;
	//adjacent[1][0] = 15;
	//adjacent[1][2] = 5;
	//adjacent[1][3] = 10;
	//adjacent[3][4] = 5;
	//adjacent[5][4] = 5;

	adjacent[0][1] = 1;
	adjacent[0][2] = 10;
	adjacent[1][3] = 1000;
	adjacent[2][3] = 10;

}

 
void Dijikstra(int here)
{
	struct VertexCost
	{
		int vertex;
		int cost;
	};

	list<VertexCost> discovered;	// 발견 목록 
	vector<int> best = vector<int>(vertices.size(), INT32_MAX);
	vector<int> parent(6, -1);

	discovered.push_back(VertexCost{here, 0});
	best[here] = 0;
	parent[here] = here;

	while (discovered.empty() == false)
	{
		// 제일 좋은 후보를 찾는다 
		list<VertexCost>::iterator bestIt;
		int bestCost = INT32_MAX;

		// 이 for문에서는 현재 discovered 중 가장 좋은 코스트를 가진 정점의 이터레이터와 그 코스트를 가져옵니다.
		for (auto it = discovered.begin(); it != discovered.end(); it++)
		{
			// 현재 까지 발견한 간선중에 가장 좋은(값이 낮은) 케이스를 갱신한다.
			if (it->cost < bestCost)
			{
				bestCost = it->cost;
				bestIt = it;
			}
		}

		// 현재 here 에서 찾은 discovered 중 베스트 코스트와 정점번호를 다시 받습니다. 
		int cost = bestCost;
		here = bestIt->vertex;
		// 왜 erase 하냐면 BFS 때 해당 정점을 방문하면서 pop 하던 부분과 비슷합니다. 
		discovered.erase(bestIt);

		// 방문? 지금 찾은 경로보다 이전 경로의 코스트가 더 좋다? 스킵
		if (best[here] < cost)
			continue;

		// 방문!
		for (int there = 0; there < 6; there++)
		{
			// 연결되지 않았으면 스킵.
			if (adjacent[here][there] == -1)
				continue;
			 
			// 더 좋은 경로를 과거에 찾았으면 스킵.
			int nextCost = best[here] + adjacent[here][there];
			if (nextCost >= best[there])
				continue;

			best[there] = nextCost;
			parent[there] = here;

			discovered.push_back(VertexCost({ there, nextCost }));
		}
	}

	int a = 0;
}

int main()
{
	CreateGraph();

	Dijikstra(0);
}