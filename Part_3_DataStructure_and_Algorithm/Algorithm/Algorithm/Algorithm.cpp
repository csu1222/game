#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>
#include <Windows.h>

// 주제 : 동적 계획법 연습문제 

// TIC-TAC-TOE

// [.][.][.]
// [.][o][x]
// [.][.][o]

vector<vector<char>> board;

enum
{
	DEFAULT = 2,
	WIN = 1, 
	DRAW = 0,
	LOSE = -1,
};

bool IsFinished(const vector<vector<char>>& board, char turn)
{
	// 좌우 
	for (int i = 0; i < 3; i++)
		if (board[i][0] == turn && board[i][1] == turn && board[i][2] == turn)
			return true;
	// 상하
	for (int i = 0; i < 3; i++)
		if (board[0][i] == turn && board[1][i] == turn && board[2][i] == turn)
			return true;
	// 대각선
	if (board[0][0] == turn && board[1][1] == turn && board[2][2] == turn)
		return true;
	if (board[0][2] == turn && board[1][1] == turn && board[2][0] == turn)
		return true;

	return false;
}

// board의 9칸에 '.', 'o', 'x' 세가지 경우의 수가 올수 있습니다. 3^9 = 19683

// Hashkey 함수로 cache에 담을 board의 상태를 정수로 표현합니다. 3진법의 원리를 응용했습니다. 
int HashKey(const vector<vector<char>>& board)
{
	int ret = 0;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			ret = ret * 3;

			if (board[y][x] == 'o')
				ret += 1;
			else if (board[y][x] == 'x')
				ret += 2;
		}
	}

	return ret;
}

// cache 를 board에 올수 있는 모든 경우의 수 만큼 크기를 줬습니다.
int cache[19683];

int CanWin(vector<vector<char>>& board, char turn)
{
	// 기저 사례
	if (IsFinished(board, 'o' + 'x' - turn))
		return LOSE;


	// 캐시 확인
	int key = HashKey(board);
	int& ret = cache[key];
	if (ret != DEFAULT)
		return ret;
	
	// 해결

	// [.][x][.]
	// [.][o][.]
	// [.][.][.]

	int minValue = DEFAULT;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (board[y][x] != '.')
				continue;
			
			// 착수 
			board[y][x] = turn;

			// 확인
			minValue = min(minValue, CanWin(board, 'o' + 'x' - turn)); // 상태방이 패배하는게 제일 좋은 케이스

			// 취소 
			board[y][x] = '.';
		}
	}

	if (minValue == DRAW || minValue == DEFAULT)
		return ret = DRAW;

	return ret = -minValue;
}

int main()
{
	board = vector<vector<char>>
	{
		{'o', '.', 'x'},
		{'o', 'o', 'x'},
		{'.', '.', '.'},
	};

	for (int i = 0; i < 19683; i++)
		cache[i] = DEFAULT;

	int win = CanWin(board, 'x');

	switch (win)
	{
	case WIN:
		cout << "Win" << endl;
		break;
	case DRAW:
		cout << "Draw" << endl;
		break;
	case LOSE:
		cout << "Lose" << endl;
		break;
	}
}