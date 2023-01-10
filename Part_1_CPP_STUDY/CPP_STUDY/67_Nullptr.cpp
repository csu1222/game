#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

// 오늘의 주제 : nullptr

/*
* 결론 : 지금까지 사용했던것 처럼 nullptr을 사용하자
* 0, NULL 과 nullptr의 차이점 :
	- 0, NULL은 정수형 0과 주소값 0이 혼동되어 사용될수 있습니다. 
	- nullptr의 정체는 객체이고 이 객체가 각 포인터들과 비교를 하게 되면 그 포인터로 캐스팅이 되면서
	반환 값은 0을 반환합니다. 그렇게 되서 정수 0으로 혼동되지 않고 템플릿 타입의 포인터로써 기능합니다.
	
	nullptr은 모던 c++에서 나타난걸 모를정도로 지금껏 
	자연스럽게 사용해왔습니다. 

	그러면 nullptr이전에는 어떻게 주소가 없다는걸 표현했을까요?
	바로 0과 NULL을 이용하였습니다.

	포인터에 값으로 0을 주면 너무 정수같이 보여서 했갈릴수 있기 때문에 NULL로도 표현을 한것입니다.
	NULL은 
	#define NULL 0; 
	이라고 그냥 0과 같은 의미지만 변수가 포인터라는걸 잘 알아보게끔 합니다.

	nullptr의 필요성
	1) 오동작
	0과 NULL의 문제점이 있으니깐 nullptr이란것이 필요했을 것입니다.
	함수 오버로딩할때 문제가 생깁니다.
	매개변수로 int를 받는 함수와 void*를 받는 함수를 오버로딩했을때
	인자로 0과 NULL을 주고 실행하면 둘다 매개변수가 int타입인 함수가 실행됩니다.
	NULL은 포인터라는걸 표시하기 위한 define이었지만 실제 값은 그냥 0으로 int로 취급이 되는것입니다.

	nullptr을 사용하면 정상적으로 포인터 버전 함수가 실행됩니다. 
	즉, 오동작 방지를 위한 의미도 있겠습니다.

	2) 가독성
	코드에서 auto 타입으로 변수를 만들었을때 이 변수의 타입을 nullptr과 비교하게 되면 바로 이 변수가 
	포인터타입 변수라는것을 알수 있습니다.


	nullptr은 여러 장단점이 있는 것들과 다르게 거의 nullptr만 사용하면 될 정도입니다.

	그러면 nullptr은 어떻게 구현되어 있길래 장점만 있는 개념일까요
	템플릿을 이용해서 연산자 오버로딩을 해주면 
	반환타입을 지정하지 않아도 됩니다.
	
	operator T* () const{return 0;} 
	으로 T라는 템플릿에는 어떤 타입이 와도 되니깐 어떤 타입의 포인터와 이 nullptr이라는 객체를 비교하면 nullptr이
	비교하는 포인터의 타입으로 캐스팅이 되고 0을 반환해줘서 사실 0과 비교하게 됩니다.
	const는 앞으로 이 연산자 오버로딩의 구현부가 바뀌지 않을것이라는 걸 뜻합니다.

	operator T C::* () const {return 0;}
	은 T와 C라는 템플릿이 있고 반환 타입이 T로 어떤 반환타입이든 C라는 어떤 클래스든지의 ::멤버 함수의 *포인터와 
	nullptr을 비교하면 nullptr이 그 멤버 함수 포인터의 타입으로 캐스팅이 되면서 0을 반환, 즉 멤버 함수포인터와 
	0을 비교하게 됩니다.	
	형태가 매우 특수하지만 예시로는 멤버 함수 포인터가 있습니다.

	그리고 
	private: 로 외부에서 접근하지못하게 
	void operator& () const;
	를 구현부없이 선언을 해버리면 nullptr의 주소에 접근을 못하게 막을수 있습니다.
	사실 private로 못쓰게 막는건 옛날 스타일이고 C++ 11 에서부터는
	void operator& () const = delete;
	이렇게 처리한다고 합니다. 


*/
// nullptr 구현하기
const
class NullPtr
{
public:
	// 그 어떤 타입의 포인터라도 치환 가능
	template<typename T>
	operator T* () const
	{
		return 0;
	}

	template<typename C, typename T>
	operator T C::* () const
	{
		return 0;
	}

private:
	void operator& () const;	// 주소값 %을 막는다 굳이 private로 막아서 접근을 막는것
} _NullPtr;	// 클래스를 선언 하자마자 객체를 만들어주는 문법


// 0과 NULL의 문제점  (함수 오버로딩)
void test(int a)
{
	cout << "test (int)" << endl;
}

void  test(void* ptr)
{
	cout << "test(*)" << endl;
}

class Knight
{
public:
	int nullptr_test(int)
	{
		cout << "member function pointer" << endl;
		return 1;
	}
public:
	int _hp;
	int* _hpPtr;
};

Knight* FindKnight()
{
	// TODO

	return _NullPtr;
}


int main()
{
	// 0과 NULL의 문제점  

	// 1) 오동작
	test(0);
	test(NULL);
	// 둘다 매개변수가 int인 함수가 실행되었습니다.
	test(_NullPtr);
	// 정상적으로 포인터 버전 함수가 실행됩니다.

	// 2) 가독성
	// auto타입으로 처리한 knight변수가 무슨 타입인지 nullptr을 보면 바로 포인터 타입인걸 알수 있습니다.
	auto knight = FindKnight();
	if (knight == _NullPtr)
	{

	}
	Knight k1;


	// 멤버 함수 포인터 
	int (Knight:: * memFUNC)(int);
	memFUNC = &Knight::nullptr_test;

	if (memFUNC == _NullPtr)
	{

	}



	return 0;
}