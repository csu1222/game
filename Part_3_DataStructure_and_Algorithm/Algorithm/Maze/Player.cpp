#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_board = board;
	_pos = _board->GetEnterPos();

	// RightHand();

	//Bfs();

	//AStar();

	AStar_2();
}

void Player::Update(int32 deltaTick)
{
	if (_pathIndex >= _path.size())
	{
		_board->GenerateMap();
		Init(_board);
		
		return;
	}
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

	map<Pos, Pos> parent;

	queue<Pos> q;

	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		if (pos == dest)
			break;

		for (int32 dir = 0; dir < 4; dir++)
		{
			Pos nextPos = pos + front[dir];

			if (CanGo(nextPos) == false)
				continue;
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}
	}

	_path.clear();

	// 거꾸로 

	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// 입구이면 break
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	// _path를 뒤집는다

	std::reverse(_path.begin(), _path.end());
}

struct PQNode
{
	bool operator<(const PQNode& other) const { return f < other.f; }
	bool operator>(const PQNode& other) const { return f > other.f; }

	int32 f;	// f = h + g
	int32 g;
	Pos pos;
};

void Player::AStar()
{
	// 점수 매기기
	// F = G + H
	// F = 최종 점수 (작을 수록 좋음, 경로에 따라 달라짐)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을 수록 좋음, 경로에 따라 달라짐)
	// H = 목적지에서 얼마나 가까운지 (작을 수록 좋은, 고정값)

	Pos start = _pos;

	Pos dest = _board->GetExitPos();

	enum
	{
		DIR_COUNT = 8,
	};

	Pos front[] =
	{
		Pos { -1 , 0 },		// Up
		Pos { 0 , -1 },		// Left
		Pos { 1 , 0 },		// Down
		Pos { 0 , 1 },		// Right
		Pos {-1, -1},		// UP_LEFT
		Pos {1, -1},		// DOWN_LEFT
		Pos {1, 1},			// DOWN_RIGHT
		Pos {-1, 1},		// UP_RIGHT
	};

	int32 cost[] =
	{
		10,		// UP
		10,		// LEFT
		10,		// DOWN
		10,		// RIGHT
		14,		//	UP_LEFT
		14,		// DOWN_LEFT
		14,		// DOWN_RIGHT
		14,		// UP_RIGHT
	};

	const int32 size = _board->GetSize();

	// ClosedList
	// closed[y][x] -> (y, x) 에 방문 했는지 여부 
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// best[y][x] -> 지금까지 (y,x) 에 대한 가장 좋은 비용(작을 수록 좋은 비용)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// 부모 추적 용도
	map<Pos, Pos> parent;

	// OpenList
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// 1) 예약(발견) 시스템 구현
	// 2) 뒤늦게 더 좋은 경로가 발견 될 수 있음 -> 예외 처리 필수 

	// 초기값
	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러경로로 찾아서 
		// 더 빠른 경로로 인해 이미 방문(Closed) 된 경우 스킵
		
		// 두가지 방법이 있는데 Closed를 이용하는 방법과 best를 이용하는 방법

		// [선택]
		if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// 방문
		closed[node.pos.y][node.pos.x] = true;
		
		// 목적지에 도달 했으면 종료
		if (node.pos == dest)
			break;

		// 갈 수 있는 모든 방향으로 체크
		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			// 각 방향으로 한걸음 더 갔을때의 Pos 구하기 
			Pos nextPos = node.pos + front[dir];

			// nextPos 가 갈 수 있는지 확인
			if (CanGo(nextPos) == false)
				continue;

			// [선택] 이미 방문한 곳이면 스킵
			// 여기서 체크해주던가 아니면 다음 while 루프때 체크해줄수 있음
			if (closed[nextPos.y][nextPos.x])
				continue;

			// nextPos 의 비용계산
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));
			// 다른 경로에서 더 빠른 길을 찾았으면 스킵
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약 진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g ,nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// 거꾸로 

	Pos pos = dest;
	
	_path.clear();
	_pathIndex = 0;


	while (true)
	{
		_path.push_back(pos);

		// 입구이면 break
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	// _path를 뒤집는다

	std::reverse(_path.begin(), _path.end());
}

void Player::AStar_2()
{
	Pos start = _pos;

	Pos dest = _board->GetExitPos();


	enum
	{
		DIR_COUNT = 8,
	};

	const int32 size = _board->GetSize();

	Pos front[] =
	{
		Pos{ -1, 0 },	// 위
		Pos{  0,-1 },	// 왼쪽
		Pos{  1, 0 },	// 아래
		Pos{  0, 1 },	// 오른쪽
		Pos{ -1,-1 },	// 왼-위
		Pos{  1,-1 },	// 왼-아래
		Pos{  1, 1 },	// 오른-아래
		Pos{ -1, 1 },	// 오른-위
	};

	int32 cost[] =
	{
		10,
		10,
		10,
		10,			// 여기까지 위,왼,아래,오른 코스트
		14,
		14,
		14,
		14,			// 여기까지는 대각선 코스트
	};



	// 클로즈 리스트
	map<Pos, bool> closed;
	//vector<vector<bool>> closed(size, vector<bool>(size, false));

	// 오픈 리스트
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>>	pq;
	// 해당 노드 까지 최선의 코스트 합
	 map<Pos, int32> best;
	//vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// 방문한 노드들의 부모 노드
	map<Pos, Pos> parent;
	// 초기값
	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));

		pq.push(PQNode{ g + h , g , start });
		//best[start.y][start.x] = g + h;
		best[start] = g + h;
 		parent[start] = start;
	}

	// while 루프

	while (pq.empty() == false)
	{
		// 최선의 노드 선택
		PQNode node = pq.top();
		pq.pop();

		// 노드가 이전에 발견했던 노드인지 
		// 이후에 발견한 노드가 혹시 코스트가 더 좋은지 체트

		/*if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;*/

		if (closed.find(node.pos) != closed.end())
			continue;
		if (best.find(node.pos) != best.end() && best[node.pos] < node.f)
			continue;

		// 방문
		// closed[node.pos.y][node.pos.x] = true;

		closed[node.pos] = true;

		// 목적지에 도달 했으면 종료
		if (node.pos == dest)
			break;

		// 방향따라 루프
		for (int dir = 0; dir < DIR_COUNT; dir++)
		{
			// 다음 방향의 노드 
			Pos nextNode = node.pos + front[dir];
			
			// 다음 노드로 갈 수 있는지 체크
			if (CanGo(nextNode) == false)
				continue;
			
			// 다음 노드의 비용 계산 
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextNode.y) + abs(dest.x - nextNode.x));
			
			// 다음 노드가 이전에 발견한것보다 좋은 코스트 인지
			/*if (best[nextNode.y][nextNode.x] <= g + h)
				continue;*/

			if (best.find(nextNode) != best.end() && best[nextNode] <= g + h)
				continue;

			// 코스트가 좋다면 오픈리스트에 추가 
			pq.push(PQNode{ g + h , g , nextNode });
			parent[nextNode] = node.pos;
			//best[nextNode.y][nextNode.x] = g + h;
			best[nextNode] = g + h;
		}
	}

	// _path에 담기 
	
	// 거꾸로 담을거임

	// 초기화 
	_path.clear();
	_pathIndex = 0;

	Pos pos = dest;

	while (true)
	{
		_path.push_back(pos);

		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());
}