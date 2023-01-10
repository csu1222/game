#include <iostream>
using namespace std;
#include <iomanip>

// 오늘의 주제 : 연습문제 (달팽이)

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
// 심화과정 : 좌표를 움직이는 코드를 더 간편화 할 수 있다
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
		밑의 스위치 문 대신 전역변수 dy, dx를 사용해 좌표를 옮길 수 있다.
		nextY = y + dy[dir];
		nextX = x + dx[dir]; 
		dir의 값을 인덱스로 사용해 좌표를 움직입니다.
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
			/*심화과정으로 dir의 방향을 좀더 간략하게 바꾸는 방법
			dir = (dir + 1) % 4;
			dir값은 각각 0,1,2,3을 가지는데 이 값들은 1씩 더해지지만 4이상으로 가면 다시 0부터 시작하도록
			해야합니다. 그래서 나머지를 구하는 연산자 %를 사용했습니다.*/
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