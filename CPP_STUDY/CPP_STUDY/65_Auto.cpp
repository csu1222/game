#include <iostream>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <vector>

// 오늘의 주제 : auto

/*
	Modern C++ (C++ 11 이후)

	이번 시간부터 모던 C++에 대해 알아봅니다. C++11은 2011년도에 나온 C++ 이란 뜻인데 매우 오래된 버전부터를 
	모던 C++이라고 부를 정도로 많은점이 달라졌습니다.

	여러 타입의 변수들을 배워왔습니다. 그런데 각각의 타입들을 맞춰주지 않고 
	auto로 바꿔도 컴파일러가 알아서 추론해서 타입을 맞춰주었습니다.

	template도 비슷하게 컴파일러에게 맞겨두는 기능이었습니다.

	이런 개념들을 유식한 말로
	형식 연역 (type deduction)
	-> 알아서 말이 되게 잘 맞춰봐~ (추론)
	이라고 합니다. 

	얼른 생각하기에는 추론이 구현하기 쉬워보이지만
	생각보다 복잡해질수 있습니다.

	무지성으로 auto를 만능키 처럼 사용하다 보면 빌드 통과가 안될 때도 있는데
	auto는 모든 상황을 다 통과시켜주는것이 아닌 상식적으로 말이 되는 상황에서 사용하는 것입니다.

	주의! 
	기본 auto는 &, const를 무시!!!
	참조값이나 const변수를 그냥 auto 변수에 대입하게 되면 &, const를 때고 인식하게됩니다.

	앞으로 타입은 무조건 auto로 통일하면 되는가? 
	- 강사님 주관적으로는 NO
	타입은 나중에 어떤 타입의 변수인지 알아보기 위한 힌트의 역할도 하고 있다.
	auto로만 되어있으면 저장되어있는 값이 무엇인지 해석해야하고 아니면 마우스 포인터를 가져다 대서 하나하나 
	확인해야합니다.
	추천 방식으로는 평소에는 원래 타입들을 사용하고
	타이핑이 길어지는 경우에 OK 타입이 되게 길게 되어있으면 어짜피 가독성이 떨어지게 될것입니다.
*/


class Knight
{
public:
	int _hp;
};


int main()
{
	
	// 지금까지 많은 타입들의 변수들을 만들어왔습니다.
	/*int a = 10;
	float b = 3.14f;
	double c = 5.55;
	Knight d = Knight();
	const char* e = "Thomas";*/


	// 모든 타입을 auto로 바꿔서 빌드를 해보면 신기하게도 통과가 됩니다!!

	auto a = 10;
	auto b = 3.14f;
	auto c = 5.55;
	auto d = Knight();
	auto e = "Thomas";

	//주의!
	//	기본 auto는&, const를 무시!!!
	int& reference = a;
	const int cst = a;

	auto test1 = reference;
	auto test2 = cst;

	// 주의 예시
	// 벡터에서 순회를 돌며 auto로 각 데이터를 수정하려고 할때
	std::vector<int> v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);

	for (int i = 0; i < v.size(); i++)
	{
		//auto data = v[i];   // int& data = v[i]; 로 했으면 원본 데이터에 접근이 됐을것
		auto& data = v[i];   // 직접 &을 붙여주면 되겠네요

		data = 100;
	}


	


	return 0;
}