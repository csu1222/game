#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_board = board;
	_pos = _board->GetEnterPos();

	Pos pos = _pos;

	// 목적지에 도착할때 까지 계속 실행 되어야 함
	Pos dest = _board->GetExitPos();

	_path.clear();
	_path.push_back(pos);


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
		if (CanGo(pos + front[rightDir]))
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

	stack<Pos> s;

	for (int i = 0; i < _path.size() - 1; i++)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
			s.pop();
		else
			s.push(_path[i]);
	}

	// 목적지 도착 
	if (_path.empty() == false)
		s.push(_path.back());

	// 스택에서 벡터로 꺼내기
	vector<Pos> path;

	while (s.empty() == false)
	{
		path.push_back(s.top());
		s.pop();
		// 스택 특성상 경로가 뒤집혀서 나옵니다.
	}
	
	// 다시 뒤집어 주기 
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
