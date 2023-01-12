#include "pch.h"
#include "Player.h"
#include "Board.h"

void Player::Init(Board* board)
{
	_board = board;
	_pos = _board->GetEnterPos();

	Pos pos = _pos;

	// �������� �����Ҷ� ���� ��� ���� �Ǿ�� ��
	Pos dest = _board->GetExitPos();

	_path.clear();
	_path.push_back(pos);

	/*
	// ��ã�� ������ ���� ���Ǵ� ��ũ��
	// ������ �������� Ʋ ���
	switch (_dir)
	{
	case DIR_UP:
		_dir = DIR_LEFT;
		break;
	case DIR_LEFT:
		_dir = DIR_DOWN;
		break;
	case DIR_DOWN:
		_dir = DIR_RIGHT;
		break;
	case DIR_RIGHT:
		_dir = DIR_UP;
		break;
	}
	// �㳪 �̷��� �ڵ尡 �ʹ� ��Ȳ����

	// �̷����� ���ؼ� ȯ�漳���� Dir �� Up ���� �ݽð� �������� ���ʴ�� ����,
	// �׸��� �� ������ ��� �ְ� ����
	// ���� switch case ���� �����ϰ� ǥ���ϸ�

	_dir = (_dir + 1) % DIR_COUNT;
	*/

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
		if (CanGo(_pos + front[rightDir]))
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
