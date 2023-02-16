#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>
#include <Windows.h>

// 주제 : 동적 계획법 연습문제 

// TRIANGLE_PATH
// - (0,0)부터 시작해서 아래 or 아래우측 으로 이동 가능
// - 만나는 숫자를 모두 더함
// - 더한 숫자가 최대가 되는 경로? 합?

// 6
// 1 2
// 3 7 4
// 9 4 1 7 
// 2 7 5 9 4

// 보드의 최대크기를 N*N 으로 만들겠습니다.
int N; 
vector<vector<int>> board;
vector<vector<int>> cache;
vector<vector<int>> nextX;

int path(int y, int x)
{
	// 기저 사항 둘 중 하나 선택
	//if (y == N - 1)
	//	return board[y][x];
	if (y == N)
		return 0;

	// 캐시 확인
	int& ret = cache[y][x];
	if (ret != -1)
		return ret;

	// 문제 적용 
	// board[y][x] + path(y + 1, x);
	// board[y][x] + path(y + 1, x + 1);

	// 경로 기록
	{
		int nextBottom = path(y + 1, x);
		int nextBottomRight = path(y + 1, x + 1);
		
		if (nextBottom < nextBottomRight)
			nextX[y][x] = x + 1;
		else
			nextX[y][x] = x;
	}

	return ret = board[y][x] + max(path(y + 1, x), path(y + 1, x + 1));

}

int path_2(int y, int x)
{
	// 기저 사항 
	if (y == N - 1)
		return board[y][x];

	// 캐시 체크
	int& ret = cache[y][x];
	if (ret != -1)
		return ret;

	// 문제 해결
	{
		int downVal = path_2(y + 1, x);
		int downRightVal = path_2(y + 1, x + 1);
		if (downVal < downRightVal)
			nextX[y][x] = x + 1;
		else
			nextX[y][x] = x;
	}

	ret = board[y][x] + max(path_2(y + 1, x), path_2(y + 1, x + 1));
	return ret;
}


int main()
{
	board = vector<vector<int>>
	{
		{6},
		{1,2},
		{3,7,4},
		{9,4,1,7},
		{2,7,5,9,4},
	};

	N = (int)board.size();
	cache = vector<vector<int>>( N, (vector<int>(N, -1)));
	nextX = vector < vector<int>>(N, vector<int>(N));

	int ret = path_2(0, 0);

	std::cout << ret << endl;

	// 경로 만들기 
	int y = 0;
	int x = 0;

	while (y < N)
	{
		cout << board[y][x] << " -> ";

		x = nextX[y][x];
		y++;
	}
}