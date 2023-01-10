#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>


// 오늘의 주제 : using


/*
	C++ 11 에서 부터 typedef 대신 사용하게된 문법입니다.

	기본형태 
	typedef TYPE NAME;	// 선언문법의 형태
	using NAME = TYPE;  // 대입 연산자 같은 형태

	typedef 과 using을 비교하면
	
	1) 직관성 (함수포인터를 예로들면)
	typedef - 선언문법에서 함수이름자리에 NAME을 넣습니다.
	using 	- 우항의 시그니쳐가 좌항이라는 이름으로 쓰겠다고 표현합니다.

	2) 템플릿
	typedef은 tempalte와 같이 사용하지 못합니다.
	- C++11 이전에는 struct나 class안에 다시 typedef을 사용해 간접적으로 사용했습니다.
	using은 template와 사용가능합니다.


	결론 : using이 typedef보다 모든면에서 사용성이 좋다.
		그래도 아직 typedef을 사용하는 코드를 읽을줄은 알아야한다

*/

// 1) 직관성
typedef void(*FUNC)();
using FUNC2 = void(*)();

// 2) 템플릿
template<typename T>
//typedef std::list<T> List; 
using List = std::list<T>;

// C++ 11 이전
template<typename T>
struct List2
{
	typedef std::list<T> type;
};


int main()
{

	// 2) 
	List<int> li;
	li.push_back(1);
	li.push_back(2);
	li.push_back(3);

	List2<int>::type li2;

	return 0;
}