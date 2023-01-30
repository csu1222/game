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

void Player::Bfs()
{
	Pos pos = _pos;

	// �������� �����Ҷ� ���� ��� ���� �Ǿ�� ��
	Pos dest = _board->GetExitPos();

	// ���� ������ ������ ũ�⸸ŭ�� Pos �迭�� ����ϴ�.
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

	// �Ųٷ� 

	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// �Ա��̸� break
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	// _path�� �����´�

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
	// ���� �ű��
	// F = G + H
	// F = ���� ���� (���� ���� ����, ��ο� ���� �޶���)
	// G = ���������� �ش� ��ǥ���� �̵��ϴµ� ��� ��� (���� ���� ����, ��ο� ���� �޶���)
	// H = ���������� �󸶳� ������� (���� ���� ����, ������)

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
	// closed[y][x] -> (y, x) �� �湮 �ߴ��� ���� 
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// best[y][x] -> ���ݱ��� (y,x) �� ���� ���� ���� ���(���� ���� ���� ���)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// �θ� ���� �뵵
	map<Pos, Pos> parent;

	// OpenList
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// 1) ����(�߰�) �ý��� ����
	// 2) �ڴʰ� �� ���� ��ΰ� �߰� �� �� ���� -> ���� ó�� �ʼ� 

	// �ʱⰪ
	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		// ���� ���� �ĺ��� ã�´�
		PQNode node = pq.top();
		pq.pop();

		// ������ ��ǥ�� ������η� ã�Ƽ� 
		// �� ���� ��η� ���� �̹� �湮(Closed) �� ��� ��ŵ
		
		// �ΰ��� ����� �ִµ� Closed�� �̿��ϴ� ����� best�� �̿��ϴ� ���

		// [����]
		if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// �湮
		closed[node.pos.y][node.pos.x] = true;
		
		// �������� ���� ������ ����
		if (node.pos == dest)
			break;

		// �� �� �ִ� ��� �������� üũ
		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			// �� �������� �Ѱ��� �� �������� Pos ���ϱ� 
			Pos nextPos = node.pos + front[dir];

			// nextPos �� �� �� �ִ��� Ȯ��
			if (CanGo(nextPos) == false)
				continue;

			// [����] �̹� �湮�� ���̸� ��ŵ
			// ���⼭ üũ���ִ��� �ƴϸ� ���� while ������ üũ���ټ� ����
			if (closed[nextPos.y][nextPos.x])
				continue;

			// nextPos �� �����
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));
			// �ٸ� ��ο��� �� ���� ���� ã������ ��ŵ
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// ���� ����
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g ,nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// �Ųٷ� 

	Pos pos = dest;
	
	_path.clear();
	_pathIndex = 0;


	while (true)
	{
		_path.push_back(pos);

		// �Ա��̸� break
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	// _path�� �����´�

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
		Pos{ -1, 0 },	// ��
		Pos{  0,-1 },	// ����
		Pos{  1, 0 },	// �Ʒ�
		Pos{  0, 1 },	// ������
		Pos{ -1,-1 },	// ��-��
		Pos{  1,-1 },	// ��-�Ʒ�
		Pos{  1, 1 },	// ����-�Ʒ�
		Pos{ -1, 1 },	// ����-��
	};

	int32 cost[] =
	{
		10,
		10,
		10,
		10,			// ������� ��,��,�Ʒ�,���� �ڽ�Ʈ
		14,
		14,
		14,
		14,			// ��������� �밢�� �ڽ�Ʈ
	};



	// Ŭ���� ����Ʈ
	map<Pos, bool> closed;
	//vector<vector<bool>> closed(size, vector<bool>(size, false));

	// ���� ����Ʈ
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>>	pq;
	// �ش� ��� ���� �ּ��� �ڽ�Ʈ ��
	 map<Pos, int32> best;
	//vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// �湮�� ������ �θ� ���
	map<Pos, Pos> parent;
	// �ʱⰪ
	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));

		pq.push(PQNode{ g + h , g , start });
		//best[start.y][start.x] = g + h;
		best[start] = g + h;
 		parent[start] = start;
	}

	// while ����

	while (pq.empty() == false)
	{
		// �ּ��� ��� ����
		PQNode node = pq.top();
		pq.pop();

		// ��尡 ������ �߰��ߴ� ������� 
		// ���Ŀ� �߰��� ��尡 Ȥ�� �ڽ�Ʈ�� �� ������ üƮ

		/*if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;*/

		if (closed.find(node.pos) != closed.end())
			continue;
		if (best.find(node.pos) != best.end() && best[node.pos] < node.f)
			continue;

		// �湮
		// closed[node.pos.y][node.pos.x] = true;

		closed[node.pos] = true;

		// �������� ���� ������ ����
		if (node.pos == dest)
			break;

		// ������� ����
		for (int dir = 0; dir < DIR_COUNT; dir++)
		{
			// ���� ������ ��� 
			Pos nextNode = node.pos + front[dir];
			
			// ���� ���� �� �� �ִ��� üũ
			if (CanGo(nextNode) == false)
				continue;
			
			// ���� ����� ��� ��� 
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextNode.y) + abs(dest.x - nextNode.x));
			
			// ���� ��尡 ������ �߰��Ѱͺ��� ���� �ڽ�Ʈ ����
			/*if (best[nextNode.y][nextNode.x] <= g + h)
				continue;*/

			if (best.find(nextNode) != best.end() && best[nextNode] <= g + h)
				continue;

			// �ڽ�Ʈ�� ���ٸ� ���¸���Ʈ�� �߰� 
			pq.push(PQNode{ g + h , g , nextNode });
			parent[nextNode] = node.pos;
			//best[nextNode.y][nextNode.x] = g + h;
			best[nextNode] = g + h;
		}
	}

	// _path�� ��� 
	
	// �Ųٷ� ��������

	// �ʱ�ȭ 
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