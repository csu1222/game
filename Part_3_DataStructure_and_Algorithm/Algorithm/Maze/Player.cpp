#include "pch.h"
#include "Player.h"
#include "Board.h"

void Player::Init(Board* board)
{
	_board = board;
	_pos = _board->GetEnterPos();

	Pos pos = _pos;

	// 목적지에 도착할때 까지 계속 실행 되어야 함
	Pos dest = _board->GetExitPos();

	_path.clear();
	_path.push_back(pos);

	/*
	// 길찾기 문제에 자주 사용되는 테크닉
	// 방향을 왼쪽으로 틀 경우
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
	// 허나 이러면 코드가 너무 장황해짐

	// 이럴때를 위해서 환경설정때 Dir 을 Up 부터 반시계 방향으로 차례대로 지정,
	// 그리고 총 갯수도 들고 있게 했음
	// 위의 switch case 문을 간단하게 표현하면

	_dir = (_dir + 1) % DIR_COUNT;
	*/

	// 앞의 방향을 방향의 크기만큼의 Pos 배열로 만듭니다.
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
		
		//	1 - 현재 바라보는 방향을 기준으로 오른쪽으로 갈 수 있는지 체크  
		if (CanGo(_pos + front[rightDir]))
		{
			//	OK.플레이어의 방향을 오른쪽으로 90도 회전 후 앞으로 한 보 전진
			_dir = rightDir;
			pos += front[_dir];

			_path.push_back(pos);
			//	No. 2번 조건으로

		}
		//2 - 현재 바라보는 방향으로 갈 수 있는지 체크
		else if (CanGo(pos + front[_dir]))
		{
			//	OK.현재 바라보는 방향으로 한 보 전진
			pos += front[_dir];
			_path.push_back(pos);
		}
		//3 - 플레이어의 방향을 왼쪽으로 90도 회전 후 처음 조건으로 루프
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
