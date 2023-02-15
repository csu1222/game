#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>
#include <Windows.h>

// 주제 : 동적 계획법 연습문제 

// LIS (Longest Increasing Sequence)

// Seq ; 1 9 2 5 7
// 부분 수열 : 일부(0개 이상) 숫자를 지우고 남은 수열 
// ex) 1 2 5 
// ex) 1 9 5 7
// 순 증가 부분 수열 
// ex) 1 2 5 

// LIS : 제일 긴 [순 증가 부분 수열]의 길이 
// 1 2 5 7 = 길이 4  

int cache[100];
vector<int> seq;

int LIS(int pos)
{
	// 기저사항 
	//if (pos == seq.size() - 1)
	//	return 1;
	
	// 캐시 확인
	int& ret = cache[pos];
	if (ret != -1)
		return ret;

	// 구하기 

	// Seq : 1 9 2 5 7

	// 최소 seq[pos]은 있으니 1부터 시작 
	ret = 1;
	
	// 1 다음으로 올 수 있는 값들은 9, 2, 5, 7  이 올 수 있습니다.

	// 1 9 = 2
	// 1 2 -> 1 2 5 7 = 4
	// 1 5 -> 1 5 7 = 3
	// 1 7 -> 1 7 = 2 

	for (int next = pos + 1; next < seq.size(); next++)
	{
		if (seq[pos] < seq[next])	// 순 증가 수열의 조건
			ret = max(ret, 1 + LIS(next));
	}


	return ret;
}

int main()
{

	__int32 start = GetTickCount64();

	::memset(cache, -1, sizeof(cache));

	::srand(((int)time(0)));

	for (int i = 0; i < 50; i++)
	{
		int randValue = rand() % 50;

		if (find(seq.begin(), seq.end(), randValue) == seq.end())
			seq.push_back(randValue);
	}

	int ret = 0;
	for (int pos = 0; pos < seq.size(); pos++)
		ret = max(ret, LIS(pos));

	__int32 end = GetTickCount64();

	cout << "ret : " << ret << endl;

	cout << "time : " << end - start << " ms" << endl;
}