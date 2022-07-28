#include <iostream>
using namespace std;
#include <iomanip>

// ������ ���� : �������� (������)

const int MAX = 100;
int board[MAX][MAX] = {};
int N;

enum DIR
{
	RIGHT,
	DOWN,
	LEFT,
	UP,
};

bool CanGo(int y, int x)
{
	if (y < 0 || y >= N)
		return false;
	if (x < 0 || x >= N)
		return false;
	if (board[y][x] != 0)
		return false;
	return true;
}
// ��ȭ���� : ��ǥ�� �����̴� �ڵ带 �� ����ȭ �� �� �ִ�
//int dy[] = { 0, 1, 0 ,-1 };
//int dx[] = { 1, 0, -1, 0 };
void SetBoard()
{
	int dir = RIGHT;
	int y = 0;
	int x = 0;
	int num = 1;

	while (true)
	{
		board[y][x] = num; 

		if (num == N * N)
			break;
		int nextY;
		int nextX;
		/*
		���� ����ġ �� ��� �������� dy, dx�� ����� ��ǥ�� �ű� �� �ִ�.
		nextY = y + dy[dir];
		nextX = x + dx[dir]; 
		dir�� ���� �ε����� ����� ��ǥ�� �����Դϴ�.
		*/
		switch (dir)
		{
		case RIGHT:
			nextY = y;
			nextX = x + 1;
			break;
		case DOWN:
			nextY = y + 1;
			nextX = x;
			break;
		case LEFT:
			nextY = y;
			nextX = x - 1;
			break;
		case UP:
			nextY = y - 1;
			nextX = x;
			break;
		}

		if (CanGo(nextY, nextX))
		{
			y = nextY;
			x = nextX;
			num++;
		}
		else
		{
			/*��ȭ�������� dir�� ������ ���� �����ϰ� �ٲٴ� ���
			dir = (dir + 1) % 4;
			dir���� ���� 0,1,2,3�� �����µ� �� ������ 1�� ���������� 4�̻����� ���� �ٽ� 0���� �����ϵ���
			�ؾ��մϴ�. �׷��� �������� ���ϴ� ������ %�� ����߽��ϴ�.*/
			switch (dir)
			{
			case RIGHT:
				dir = DOWN;
				break;
			case DOWN:
				dir = LEFT;
				break;
			case LEFT:
				dir = UP;
				break;
			case UP:
				dir = RIGHT;
				break;
			}
		}
	}


}
void PrintBoard()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			cout << setfill('0') << setw(2) << board[y][x] << " ";
		}
		cout << endl;
	}
}

int main()
{
	cin >> N;

	SetBoard();


	PrintBoard();

	return 0;
}