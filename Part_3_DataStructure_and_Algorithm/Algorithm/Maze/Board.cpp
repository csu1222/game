#include "pch.h"
#include "Board.h"
#include "Player.h"
#include "DisjointSet.h"

// unicode-table.com/kr/ ���� �����Խ��ϴ�. �˻���:U+25A0
const char* TILE = "��";

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

// Binary Tree �̷� ���� �˰��� 
// - Maze for Programers ��� å���� ���� �˰���
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

	// edges �ĺ��� ���� cost�� ����ϴ� �ڵ� 
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			// ���ΰ��� ��ŵ 
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			// ���� �����ϴ� ���� �ĺ� 
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.push_back(CostEdge{ randValue, Pos{ y, x}, Pos{ y, x + 2} });
			}
			// �Ʒ��� �����ϴ� ���� �ĺ� 
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.push_back(CostEdge{ randValue, Pos{ y, x}, Pos{ y + 2, x} });
			}
		}
	}
	
	// �ϴ� ��� ���� edges �� sort �մϴ�. 
	std::sort(edges.begin(), edges.end());

	DisjointSet sets(_size * _size);

	for (CostEdge& edge : edges)
	{
		// edge�� u v �� Pos �̴� ���� sets���� ����� �� �ֵ��� �ε��� ������ ��ȯ 
		// �� ������ 2���� �迭�� 1�������� �����ϰ� ������ ����ϴ� ����
		int u = edge.u.y * _size + edge.u.x;
		int v = edge.v.y * _size + edge.v.x;

		// ���� �׷��̸� ��ŵ 
		if (sets.Find(u) == sets.Find(v))
			continue;

		// �� �׷��� ��ģ�� 
		sets.Merge(u, v);

		// �ʿ� ����  

		// Ÿ���� ��ǥ�� u �� v�� ���� Ÿ���ϰ̴ϴ�. �̶� edge�� �Ʒ��� �������� ������ �������� 
		// �𸣴� �׳� �߰����� ���մϴ�. 
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