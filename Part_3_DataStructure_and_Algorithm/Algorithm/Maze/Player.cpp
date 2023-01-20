#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_board = board;
	_pos = _board->GetEnterPos();

	// RightHand();

	Bfs();
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

void Player::RightHand()
{

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

void Player::Bfs()
{
	Pos pos = _pos;

	// 목적지에 도착할때 까지 계속 실행 되어야 함
	Pos dest = _board->GetExitPos();

	// 앞의 방향을 방향의 크기만큼의 Pos 배열로 만듭니다.
	Pos front[DIR_COUNT] =
	{
		Pos { -1 , 0 },		// Up
		Pos { 0 , -1 },		// Left
		Pos { 1 , 0 },		// Down
		Pos { 0 , 1 },		// Right
	};

	const int32 size = _board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));

	// vector<vector<Pos>> parent;
	// parent[A] = B; A는 B로 인해 발견함
	map<Pos, Pos> parent;

	queue<Pos> q;

	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		// 방문! 
		
		// 목적지라면 break
		if (pos == dest)
			break;

		// 위,왼쪽,아래,오른쪽 의 방향각각 을 체크 
		for (int32 dir = 0; dir < 4; dir++)
		{	
			Pos nextPos = pos + front[dir];
			
			// nextPos 가 갈수 있는 타일인지?
			if (CanGo(nextPos) == false)
				continue;

			// nextPos 가 갈 수 있다면 이미 탐색했던 타일인지?
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}

	}


	_path.clear();

	// 거꾸로 거슬러 올라간다

	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// 부모와 자신이 같다면 시작점이다
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	// 거꾸로 거슬러 올라간걸 다시 뒤집는다.
	std::reverse(_path.begin(), _path.end());
}
