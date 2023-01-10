#include <iostream>
using namespace std;

// 연습 문제 구구단 

int main()
{
	// 구구단
	// 2 * 1 = 2
	// 2단부터 9단까지 구구단을 출력해주세요!	

	for (int i = 2; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			cout << i << "*" << j + 1 << "=" << i * j << endl;
		}
	}
	
}