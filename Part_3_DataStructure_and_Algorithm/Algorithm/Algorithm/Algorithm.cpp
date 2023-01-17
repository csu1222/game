#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

void CreateGraph_1()
{
	struct Vertex
	{
		vector<Vertex*> edges;
	};

	vector<Vertex> v;
	v.resize(6); 

	v[0].edges.push_back(&v[1]);
	v[0].edges.push_back(&v[3]);
	v[1].edges.push_back(&v[0]);
	v[1].edges.push_back(&v[2]);
	v[1].edges.push_back(&v[3]);
	v[3].edges.push_back(&v[4]);
	v[5].edges.push_back(&v[4]);

	// Q. 0번 -> 3번 정점이 연결되어 있나요?
	bool connected = false;
	for (Vertex* edge : v[0].edges)
	{
		if (edge == &v[3])
		{
			connected = true;
			break;
		}
	}
};


void CreateGraph_2()
{
	struct Vertex
	{
		// int _data;
	};

	vector<Vertex> v;
	v.resize(6);


	vector<vector<int>> adjacent(6);

	adjacent[0] = { 1, 3 };
	adjacent[1] = { 0, 2, 3 };
	adjacent[3] = { 4 };
	adjacent[5] = { 4 };

	// Q. 0번 -> 3번 정점이 연결되어 있나요?
	bool connected = false;
	for (int vtx : adjacent[0])
	{
		if (vtx == 3)
		{
			connected = true;
			break;
		}
	}
	// STL
	vector<int> adj = adjacent[0];

	bool connected_2 = (std::find(adj.begin(), adj.end(), 3) != adj.end());

	// 가중치 표현 
	pair<int, int> ;

	vector<vector<pair<int, int>>> adjacent2(6);

	adjacent2[0] = { { 1,15 } , { 3, 35 } };
	adjacent2[1] = { { 0, 15 } , { 2, 5 } , { 3 , 10 } };
	adjacent2[3] = { { 4, 5 } };
	adjacent2[5] = { { 4, 5 } };

	// Q. 0번 -> 3번 정점이 연결되어 있나요?
	bool WeightConnected = false;
	for (pair<int, int> vtx : adjacent2[0])
	{
		if (vtx.first == 3)
		{
			connected = true;
			break;
		}
	}
	// STL
	vector<pair<int, int>> WeightAdj = adjacent2[0];

	bool WeightConnected_2 = (std::find(adj.begin(), adj.end(), 3) != adj.end());

};

void CreateGraph_3()
{
	struct Vertex
	{
		// int _data;
	};

	vector<Vertex> v;
	v.resize(6);

	// 연결된 목록을 행렬로 관리 
	//	[X][O][X][O][X][X]
	//	[O][X][O][O][X][X]
	//	[X][X][X][X][X][X]
	//	[X][X][X][X][O][X]
	//	[X][X][X][X][X][X]
	//	[X][X][X][X][O][X]

	// 읽는 방법 : adjacent[form][to]
	// 행렬을 이용한 그래프 표현(2차원 배열)
	// 메모리 소모가 심하지만 빠른 접근이 가능
	// (간선이 많은 경우 이점이 있다.)
	vector<vector<bool>> adjacent(6, vector<bool>(6, false));

	adjacent[0][1] = true;
	adjacent[0][3] = true;
	adjacent[1][0] = true;
	adjacent[1][2] = true;
	adjacent[1][3] = true;
	adjacent[3][4] = true;
	adjacent[5][4] = true;

	// Q. 0번 -> 3번 정점이 연결되어 있나요?
	bool connected = adjacent[0][3];

	// 가중치 표현 
	vector<vector<int>> adjacent2 = 
	{
		vector<int> { -1, 15, -1, 35, -1, -1},
		vector<int> { 15, -1, +5, 10, -1, -1},
		vector<int> { -1, -1, -1, -1, -1, -1},
		vector<int> { -1, -1, -1, -1, +5, -1},
		vector<int> { -1, -1, -1, -1, -1, -1},
		vector<int> { -1, -1, -1, -1, +5, -1},
	};

	// Q. 0번 -> 3번 정점이 연결되어 있나요?
	bool connected2 = adjacent2[0][3] > -1;

	// Q2. 0번 -> 4번의 가중치는 얼마인가요?
	int weight = adjacent2[0][4];
};

int main()
{
	CreateGraph_1();
	CreateGraph_2();
}