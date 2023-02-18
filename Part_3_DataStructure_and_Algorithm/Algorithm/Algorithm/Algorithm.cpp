#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>
#include <Windows.h>

// 주제 : 동적 계획법 연습문제 

// ENCHANT

// +0 집행검
// 무기 강화 주문서 +1 / +2 / +3

// +9 집행검 뜨는 경우의 수
// ex) +1 +2 +3 ... +9
// ex) +3 +6 +9
// ex) +1 +3 +4 ....

int N;
int count = 0;
vector<int> cache;	

// [+num]에서 시작해서, [+N]까지 가는 경우의 수 
int Enchant(int num)
{
	// 기저사항
	if (num > N)
		return 0;
	if (num == N)
		return 1;

	//캐시 확인
	int& ret = cache[num];
	if (ret != -1)
		return ret;

	// 문제 해결 
	return ret = Enchant(num + 1) + Enchant(num + 2) + Enchant(num + 3);
}

int main()
{
	srand((int)time(0));
	N = 9;
	cache = vector<int>(N, -1);

	int ret = Enchant(0);
	cout << ret << endl;

}