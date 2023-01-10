#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
using namespace std;

// 연습 문제

// 밑의 연습문제들을 풀어봤는데 그때 그때 짠 이 코드들이 딱히 문제가 있지는않지만 최선은 아닙니다.
// 다른 사람이 딱 처음 봤다면 아무리 간단한 코드라도 몇초는 코드를 해석하는 시간이 걸릴 것이고
// 또 문제의 전제인 vector를 기준으로 짠 코드이기 때문에 다른 컨테이너를 대입해서 쓰려면 호환이 염려됩니다.
// 그래서 C++에서 정식으로 지원하는 기능들을 쓴다면 모든 C++ 프로그래머가 공통된 형태의 코드를 가독성있게
// 받아들일수 있을 것입니다. 
// 다음 강의인 algorithm에서 배워보겠습니다.
//

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// 문제 1) number라는 숫자가 벡터에 있는지 체크하는 기능 (bool, 첫 등장 iterator)
	{
		int number = 50;

		bool found = false;
		vector<int>::iterator checkIt = v.end();

		// TODO
		for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		{
			int value = *it;
			if (value == number)
			{
				found = true;
				checkIt = it;
				break;
			}
		}
	}

	// 문제 2) 11로 나뉘는 숫자가 벡터안에 들어가 있는지 체크 (bool, 첫 등장 iterator)
	{
		bool found = false;
		vector<int>::iterator checkIt = v.end();


		for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		{
			int value = *it;
			if (value % 11 == 0)
			{
				found = true;
				checkIt = it;
				break;
			}
		}
		
	}


	// 문제 3) 홀수인 숫자의 갯수는? (count)
	{
		int count = 0;

		for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		{
			int value = *it;
			if (value % 2 == 1)
			{
				count++;
			}
		}
	}

	// 문제 4) 벡터에 들어가 있는 모든 숫자들에 3을 곱해주세요
	{
		for (int i = 0; i < v.size(); i++)
		{
			
			v[i] *= 3;
		}
	}


	return 0;
}