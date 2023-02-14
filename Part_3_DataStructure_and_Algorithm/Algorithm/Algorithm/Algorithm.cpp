#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>
#include <Windows.h>

// 주제 : 동적 계획법 

// 이항 계수 Combination

// 메모이제이션 (memoization)
// n이 최대 50 인 경우를 상정할겁니다. cache 의 범위는 [n의 범위][r의 범위] 를 뜻합니다. 
// cache에 이미 구한 답이 있는지를 알아 보기위해 같은 크기의 bool 이차 배열을 만드는것은 메모리 낭비입니다.
// 그래서 초기 값으로 절대 오지 못할 값을 넣어 두고 그걸로 판단합니다. 
int cache[50][50];

int combination(int n, int r)
{
    // 기저 사례
    if (r == 0 || n == r)
        return 1;

    // 이미 답을 구한 적이 있으면 바로 반환 
    int& ret = cache[n][r];
    if (ret != -1)
        return ret;

    // 새로 답을 구해서 캐시에 저장

    return ret = combination(n - 1, r - 1) + combination(n - 1, r);
}

int main()
{
    // cache를 -1로 초기화 
    ::memset(cache, -1, sizeof(cache));

    __int64 start = GetTickCount64();

    int lotto = combination(45, 6);

    __int64 end = GetTickCount64();

    cout << end - start << "ms" << endl;

}