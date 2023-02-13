#include "pch.h"
#include "Board.h"
#include "Player.h"
#include "DisjointSet.h"

// unicode-table.com/kr/ 에서 가져왔습니다. 검색어:U+25A0
const char* TILE = "■";

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init(int32 size, Player* player)
{
	_size = size;
	_player = player;

	//GenerateMap_Kruskal();
	GenerateMap_Prim();
}

void Board::Render()
{
	ConsoleHelper::SetCursorPosition(0, 0);
	ConsoleHelper::ShowConsoleCursor(false);

	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			ConsoleColor color = GetTileColor(Pos{ y, x });
			ConsoleHelper::SetCursorColor(color);
			cout << TILE;
		}

		cout << endl;
	}
}

// Binary Tree 미로 생성 알고리즘 
// - Maze for Programers 라는 책에서 나온 알고리즘
//
void Board::GenerateMap_Kruskal()
{
	struct CostEdge
	{
		int cost;
		Pos u;
		Pos v;

		bool operator<(CostEdge& other)
		{
			return cost < other.cost;
		}
	};

	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				_tile[y][x] = TileType::WALL;
			else
				_tile[y][x] = TileType::EMPTY;
		}
	}

	vector<CostEdge> edges;

	// edges 후보를 랜덤 cost로 등록하는 코드 
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			// 벽인곳은 스킵 
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			// 우측 연결하는 간선 후보 
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.push_back(CostEdge{ randValue, Pos{ y, x}, Pos{ y, x + 2} });
			}
			// 아래로 연결하는 간선 후보 
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.push_back(CostEdge{ randValue, Pos{ y, x}, Pos{ y + 2, x} });
			}
		}
	}
	
	// 일단 모두 담은 edges 를 sort 합니다. 
	std::sort(edges.begin(), edges.end());

	DisjointSet sets(_size * _size);

	for (CostEdge& edge : edges)
	{
		// edge의 u v 가 Pos 이다 보니 sets에서 사용할 수 있도록 인덱스 값으로 변환 
		// 이 공식은 2차원 배열을 1차원으로 관리하고 싶을때 사용하는 공식
		int u = edge.u.y * _size + edge.u.x;
		int v = edge.v.y * _size + edge.v.x;

		// 같은 그룹이면 스킵 
		if (sets.Find(u) == sets.Find(v))
			continue;

		// 두 그룹을 합친다 
		sets.Merge(u, v);

		// 맵에 적용  

		// 타일의 좌표는 u 와 v의 사이 타일일겁니다. 이때 edge가 아래쪽 간선인지 오른쪽 간선인지 
		// 모르니 그냥 중간값을 구합니다. 
		int32 y = (edge.u.y + edge.v.y) / 2;
		int32 x = (edge.u.x + edge.v.x) / 2;
		_tile[y][x] = TileType::EMPTY;
	}
}

void Board::GenerateMap_Prim()
{
	struct CostEdge
	{
		int cost;
		Pos vtx;

		bool operator<(const CostEdge& other) const
		{
			return cost < other.cost;
		}
	};

	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				_tile[y][x] = TileType::WALL;
			else
				_tile[y][x] = TileType::EMPTY;
		}
	}

	// edges[u] : u 정점과 연결된 간선 목록 
	map<Pos, vector<CostEdge>>  edges;

	// edges 후보를 랜덤으로 등록 
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			// 벽인곳은 스킵 
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			// 우측 연결하는 간선 후보 
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{ y, x };
				Pos v = Pos{ y, x + 2 };
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}
			// 아래로 연결하는 간선 후보 
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{ y, x };
				Pos v = Pos{ y + 2, x };
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}
		}
	}

	// 해당 정점이 트리에 포함되어 있나?
	map<Pos, bool> added;
	// 어떤 정점이 누구에 의해 연결 되었는지?
	map<Pos, Pos> parent;
	// 만들고 있는 트리에 인접한 간선 중, 해당 정점에 닿는 최소 간선의 정보
	map<Pos, int32> best;

	// 다익스트라와 거의 유사함 단!
	// - 다익스트라에서는 best가 [시작점]을 기준으로한 cost
	// - 프림에서는 best가 [현재 트리]를 기준으로 한 간선 cost

	// best를 int32 가장 큰값으로 added 를 false 로 초기화
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			best[Pos{ y, x }] = INT32_MAX;
			added[Pos{ y, x }] = false;
		}
	}

	priority_queue<CostEdge> pq;
	const Pos startPos = Pos{ 1, 1 }; // 랜덤으로 정해도 됨
	pq.push(CostEdge{ 0 , startPos });
	best[startPos] = 0;
	parent[startPos] = startPos;

	while (pq.empty() == false)
	{
		CostEdge bestEdge = pq.top();
		pq.pop();

		// 새로 연결된 정점
		Pos v = bestEdge.vtx;
		// 이미 추가되었다면 스킵 
		if (added[v])
			continue;

		added[v] = true;

		// 맵에 적용
		{
			int y = (parent[v].y + v.y) / 2;
			int x = (parent[v].x + v.x) / 2;
			_tile[y][x] = TileType::EMPTY;
		}

		// 방금 추가한 정점의 인접한 간선들을 검사 
		for (CostEdge& edge : edges[v])
		{
			// 이미 추가 되었으면 스킵
			if (added[edge.vtx])
				continue;

			// 다른 경로로 더 좋은 후보가 발견 되었으면 스킵
			if (edge.cost > best[edge.vtx])
				continue;

			best[edge.vtx] = edge.cost;
			parent[edge.vtx] = v;
			pq.push(edge);
		}
	}
}

TileType Board::GetTileType(Pos pos)
{
	if (pos.x < 0 || pos.x > _size)
		return TileType::NONE;

	if (pos.y < 0 || pos.y > _size)
		return TileType::NONE;

	return _tile[pos.y][pos.x];
}

ConsoleColor Board::GetTileColor(Pos pos)
{
	if (_player && _player->Getpos() == pos)
		return ConsoleColor::YELLOW;

	if (GetExitPos() == pos)
		return ConsoleColor::BLUE;

	TileType tileType = GetTileType(pos);

	switch (tileType)
	{
	case TileType::EMPTY:
		return ConsoleColor::GREEN;
	case TileType::WALL:
		return ConsoleColor::RED;
	}

	return ConsoleColor::WHITE;
}