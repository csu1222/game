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

	GenerateMap();
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
void Board::GenerateMap()
{
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