#include <iostream>
using namespace std;

// 오늘의 주제 : 호출 스택
// - 프로젝트를 진행하다보면 수많은 함수들이 생길것이고 
// - 모든 함수를 외우고 있다는것은 말이 안 될 것입니다.
// - 스택프레임에서 배웠듯이 디버그 로 메모리를 하나씩 확인하는것도 매우 번거롭습니다.
// - 그래서 대부분의 디버그 도구에서 호출 스택이라는 기능을 지원하고 있다고 합니다.


#pragma region 함수 선언

// 번외 : 함수 선언
// - 여러 함수를 만들었고 함수내에서 다른 함수들을 호출하고 싶을 때 
// - 밑과 같은 예제를 들어 보겠습니다.
// 빌드에서 Func2, Func3의 식별자를 찾을 수 없다고 오류가 났습니다.
// C++에서는 컴파일을 할 때 첫째 줄 부터 차근차근 컴파일을 하기 때문에 
// Func1에서 Func2를 부르면 그런 이름의 함수를 못찾는다고 할 수 밖에 없습니다.
// 그러면 매번 함수위치를 수작업으로 바꿔가면서 개발해야 하는가?
// 코드의 윗 부분에 미리 간이적으로 '함수 선언'을 해주면 됩니다.
// 함수선언을 할때 자세한 구현부는 생략해도 되겠습니다.
void Func1();
void Func2(int a, int b);  // 함수 선언 할때 매개변수는 구현부와 동일하지 않아도 됩니다. 어떤 구조인지만 알수 있으면 됩니다.
// ex) Func2(int, int); <-(가능) , Func2(int hp, int mp); <-(가능)
void Func3(float a);
#pragma endregion

#pragma region 함수 호출 예제
void Func1()
{
	cout << "Func1" << endl;
	// push 2
	// push 1
	// call Func2		어셈블리로 볼때 이 단계에서 call할 대상을 못찾게 됩니다.
	Func2(1, 2);

	Func3(0.1);
}

void Func2(int a, int b)
{
	cout << "Func2" << endl;

	Func3(10);
}

void Func3(float a)
{
	cout << "Func3" << endl;
}
#pragma endregion
int main()
{

	cout << "main" << endl;
	Func1();
}