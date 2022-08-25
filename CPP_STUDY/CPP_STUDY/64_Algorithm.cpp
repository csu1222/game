#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
using namespace std;

#include <algorithm>

// 오늘의 주제 : 알고리즘

/*
	이번시간에는 이전에 풀었던 연습문제를 좀더 권장되는 방식으로 플어보겠습니다.

	프로그래밍을 배우다보면 이 두가지 용어를 자주 들어볼것입니다.
	
	자료구조 (데이터를 저장하는 구조)
	알고리즘 (데이터를 어떻게 사용할 것인가?)

	C++에서 지원하는 알고리즘 라이브러리는 그 양이 너무 많아서 모든 알고리즘을 전부 이해하면서
	사용할 필요는 없지만 자주 사용하는 알고리즘들은 잘 알아두면 좋을것입니다.

	find
	find_if
	count
	count_if
	all_of
	none_of
	for_each
	remove
	remove_if


	 문제 1) number라는 숫자가 벡터에 있는지 체크하는 기능 (bool, 첫 등장 iterator)
	 find 라는 알고리즘을 사용하면 간편하게 기능을 사용할수 있습니다.
	 find는 인자를 3개받는데요 첫번째 인자로 받은 이터레이터와 두번째 인자로 받은 이터레이터 사이에서 
	 세번째 인자로 받은 값을 찾아줍니다. 반환은 찾은 값의 이터레이터입니다.

	 문제 2) 11로 나뉘는 숫자가 벡터안에 들어가 있는지 체크 (bool, 첫 등장 iterator)
	 find_if라는 알고리즘으로 더욱 간결하게 구현할수 있습니다.
	 find 처럼 첫번째 두번째 인자로 찾을 구간을 설정하고, 세번째 인자로 어떤 조건으로 찾을것인지를 주면 됩니다.
	 조건은 함수처럼 ()연산자를 사용할수 있어야하고 인자로 찾을 값을 주는 시그니쳐여야 합니다. 
	 struct CanByDivid11
		{
			bool operator() (int n)
			{
				return (n % 11) == 0;
			}
		};
	이렇게 함수객체로 구현하였고 
	find_if(v.begin(), v.end(), CanByDivid11());
	이렇게 주면 콜백 함수의 인자는 알아서 범위를 순회하면서 넣어줄 것입니다.


	문제 3) 홀수인 숫자의 갯수는? (count)
	count와 count_if의 차이점은 count는 어떤 값 자체를 세는 것이고, count_if는 어떤 조건을 만족하는 값들을 세는 것입니다.
	위 find와 find_if 처럼 사용하면 되겠습니다. 다른점은 반환값이 find들은 이터레이터고 count들은 int라는 것입니다.

	추가로 비슷하게 사용되는 것들을 더 소개 하면
	
	모든 데이터가 (조건) 입니까?
	std::all_of(begin(), end(), (조건));
	
	데이터중에 (조건)이 하나라도 있습니까?
	std::any_of(begin(), end(), (조건));
	
	모든 데이터가 (조건)이 아닙니까?
	std::none_of(begin(), end(), (조건));


	문제 4) 벡터에 들어가 있는 모든 숫자들에 3을 곱해주세요
	for_each를 사용합니다. for_each는 문제 처럼 각 데이터마다 어떤 일을 하는데도 사용되지만, 그냥 모든 데이터를 순회하는데에도 
	사용됩니다. 

	
	처음에 예시를 든 함수들중 remove, remove_if가 남았습니다. 이 둘은 다른 함수들과 조금 다르게 동작한다고 합니다.


	문제 5) 홀수인 데이터를 일괄 삭제해주세요
	remove_if를 사용해야합니다. 여태껏 사용한 바로는 바로 뚝딱 사용할수 있을것 같지만 그 결과를 보면 아주 이산한 벡터가 
	나옵니다. 예시로 1, 3, 4, 6, 7, 8 이라는 데이터를 가진 벡터에게 
	std::remove_if(벡터.begin(), 벡터.end(), IsOdd());
	이렇게 주었습니다. 그 결과 벡터는 4, 6, 8, 6, 7, 8 이라는 값을 가지게 되었습니다. 그 이유는 remove 류 함수들이 알아서 
	벡터에서 조건에 맞지 않는 결과들을 지우는 것이 아닌 조건에 맞는 데이터들을 원본 벡터에 복사해놓는 것이었습니다. 
	그 이유는 벡터가 중간 값 삭제를 하면 매 삭제마다 전체 데이터를 이동해야 했지만 반대로 조건에 맞지않는, 남겨놓고싶은 
	데이터를 하나씩 복사 해놓는것이 더 빠르기 때문입니다. 데이터를 복사한후의 이터레이터를 반환합니다.
	그래서 원하는 결과를 얻기 위해서는 remove, remove_if이후 같이 erase로 이후 데이터들을 지워주면 되겠습니다.
	벡터.erase(std::remove_if(벡터.begin(), 벡터.end(), IsOdd()), 벡터.end());


*/

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

		vector<int>::iterator itFind = find(v.begin(), v.end(), number);
		if (itFind == v.end())
		{
			cout << "못 찾았음" << endl;
		}
		else
		{
			cout << "찾았음" << endl;
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

		struct CanByDivid11
		{
			bool operator() (int n)
			{
				return (n % 11) == 0;
			}
		};

		vector<int>::iterator itFind = find_if(v.begin(), v.end(), CanByDivid11());
		if (itFind == v.end())
		{
			cout << "못 찾았음" << endl;
		}
		else
		{
			cout << "찾았음" << endl;
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

		struct IsOdd
		{
			bool operator() (int n)
			{
				return (n % 2) != 0;
			}
		};

		int cnt = count_if(v.begin(), v.end(), IsOdd());

		cout << "count_if = " << cnt << endl;
	}

	// 문제 4) 벡터에 들어가 있는 모든 숫자들에 3을 곱해주세요
	{
		for (int i = 0; i < v.size(); i++)
		{

			v[i] *= 3;
		}

		struct MultipleBy3
		{
			void operator() (int& n)
			{
				n *= 3;
			}
		};

		std::for_each(v.begin(), v.end(), MultipleBy3());
	}

	// 문제 5) 홀수인 데이터를 일괄 삭제해주세요
	{
		/*for (vector<int>::iterator it = v.begin(); it != v.end();)
		{
			int num = *it;
			if ((num % 2) != 0)
				it = v.erase(it);
			else
				++it;
		}*/

		// 데이터를 살펴보기 쉽게 간추려 보겠습니다.
		v.clear();

		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(7);
		v.push_back(10);
		v.push_back(8);

		// remove로 어떤 값을 삭제
		//std::remove(v.begin(), v.end(), 2);

		// remove_if로 조건으로 삭제

		struct IsOdd
		{
			bool operator() (int n)
			{
				return (n % 2) != 0;
			}
		};

		vector<int>::iterator it = std::remove_if(v.begin(), v.end(), IsOdd());
		v.erase(it, v.end());

	}

	return 0;
}