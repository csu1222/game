#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_board = board;
	_pos = _board->GetEnterPos();

	Pos pos = _pos;

	// �������� �����Ҷ� ���� ��� ���� �Ǿ�� ��
	Pos dest = _board->GetExitPos();

	_path.clear();
	_path.push_back(pos);


	// ���� ������ ������ ũ�⸸ŭ�� Pos �迭�� ����ϴ�.
	Pos front[DIR_COUNT] =
	{
		Pos { -1 , 0 },		// Up
		Pos { 0 , -1 },		// Left
		Pos { 1 , 0 },		// Down
		Pos { 0 , 1 },		// Right
	};
	
	while (pos != dest)
	{
		int32 rightDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		
		//	1 - ���� �ٶ󺸴� ������ �������� ���������� �� �� �ִ��� üũ  
		if (CanGo(pos + front[rightDir]))
		{
			//	OK.�÷��̾��� ������ ���������� 90�� ȸ�� �� ������ �� �� ����
			_dir = rightDir;
			pos += front[_dir];

			_path.push_back(pos);
			//	No. 2�� ��������

		}
		//2 - ���� �ٶ󺸴� �������� �� �� �ִ��� üũ
		else if (CanGo(pos + front[_dir]))
		{
			//	OK.���� �ٶ󺸴� �������� �� �� ����
			pos += front[_dir];
			_path.push_back(pos);
		}
		//3 - �÷��̾��� ������ �������� 90�� ȸ�� �� ó�� �������� ����
		else
		{
			_dir = (_dir + 1) % DIR_COUNT;
		}
	}

	stack<Pos> s;

	for (int i = 0; i < _path.size() - 1; i++)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
			s.pop();
		else
			s.push(_path[i]);
	}

	// ������ ���� 
	if (_path.empty() == false)
		s.push(_path.back());

	// ���ÿ��� ���ͷ� ������
	vector<Pos> path;

	while (s.empty() == false)
	{
		path.push_back(s.top());
		s.pop();
		// ���� Ư���� ��ΰ� �������� ���ɴϴ�.
	}
	
	// �ٽ� ������ �ֱ� 
	std::reverse(path.begin(), path.end());

	_path = path;

}

void Player::Update(int32 deltaTick)
{
	if (_pathIndex >= _path.size())
		return;

	_sumTick += deltaTick;

	if (_sumTick >= MOVE_TICK)
	{
		_sumTick = 0;
		
		_pos = _path[_pathIndex];

		_pathIndex++;
	}
}

bool Player::CanGo(Pos pos)
{
	TileType tileType = _board->GetTileType(pos);

	return tileType == TileType::EMPTY;
}
