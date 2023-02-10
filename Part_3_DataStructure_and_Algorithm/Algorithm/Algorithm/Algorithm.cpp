#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>

// 주제 : 최소 스패닝 트리 Minimum Spanning Tree


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

struct Vertex
{
	// int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent; // 인접 행렬

void CreateGraph()
{
	vertices.resize(6);
	adjacent = vector<vector<int>>(6, vector<int>(6, -1));	// 초기값인 -1 짜리 간선은 연결되지 않은 간선

	adjacent[0][1] = adjacent[1][0] = 15;
	adjacent[0][3] = adjacent[3][0] = 35;
	adjacent[1][2] = adjacent[2][1] = 5;
	adjacent[1][3] = adjacent[3][1] = 10;
	adjacent[3][4] = adjacent[4][3] = 5;
	adjacent[3][5] = adjacent[5][3] = 10;
	adjacent[5][4] = adjacent[4][5] = 5;
}

// u 에서 v로 가고 그 코스트를 나타낼 구조체
struct CostEdge
{
	int cost;
	int u;
	int v;

	bool operator<(CostEdge& other)
	{
		return cost < other.cost;
	}
};

// 쿠르스칼 알고리즘 반환하는것은 완성된 최소 스패닝 트리의 코스트 합
int Kruskal(vector<CostEdge>& selected)
{
	int ret = 0;

	selected.clear();

	// edges 에 그래프의 간선들을 긁어오고 코스트가 좋은 순서대로 정렬
	vector<CostEdge> edges;

	for (int u = 0; u < adjacent.size(); u++)
	{
		for (int v = 0; v < adjacent[u].size(); v++)
		{
			// 간선이 중복되지않도록 체크 
			if (u > v)
				continue;

			int cost = adjacent[u][v];
			if (cost == -1)
				continue;

			edges.push_back(CostEdge{ cost, u, v });
		}
	}

	std::sort(edges.begin(), edges.end());
	

	// 다음은 크루스칼 알고리즘대로 코스트가 좋은 간선들을 연결하기 시작하는데 
	// 거기에 Disjoint Set 을 활용한 그룹 관리추가 

	DisjointSet sets(vertices.size());

	for (CostEdge& edge : edges)
	{
		// 같은 그룹이면 스킵 (안 그러면 사이클 발생)
		if (sets.Find(edge.u) == sets.Find(edge.v))
			continue;

		// 두 그룹을 합친다  
		sets.Merge(edge.u, edge.v);
		selected.push_back(edge);
		ret += edge.cost;
	}

	return ret;
}

int main()
{
	CreateGraph();

	vector<CostEdge> selected;
	int cost = Kruskal(selected);

}