#include <iostream>
using namespace std;

// 오늘의 주제 : 함수 마무리

#pragma region 1)오버로딩
// 1) 오버로딩 (중복 정의 : 함수 이름의 재사용)
// - 매개변수 갯수가 다르다거나
// - 매개변수 타입이 다르다거나 (순서가 다른걸 포함)
// - 하는 구별이 가능하다면 같은 이름의 함수를 정의해도 된다.
// - 함수의 반환 타입으로만 구분해서 오버로딩 할 수는 없다.
int Add(int a, int b)  // int 타입을 더하는 함수
{
	int result = a + b;
	return result;
}

float Add(float a, float b)  // float 타입을 더하는 함수
{
	float result = a + b;
	return result;
}

#pragma endregion

#pragma region 2)기본인자
// 기본 인자값
 // - 함수에서 미리 정해주는 인자중에 생략할 인자는 자동으로 기본값을 할당하는 기능
// - 기본 인자로 설정 할 때는 순서상 맨 뒤에 위치해야합니다. 뒤에 만 몰아 둔다면 갯수는 상관없습니다.
void SetPlayerInfo(int hp, int mp, int attack, int gulidid = 0, int catleid = 0)  // 길드 아이디는 없을 수도 있으니 기본 인자값으로 0을 주었습니다.
{

}
#pragma endregion

#pragma region 3)스택오버플로우
// 스택 오버플로우
// - 스택은 함수에서 사용되는 메모리인데 함수안에 다시 함수를 호출 하면 스택에 데이터가 점점 쌓이게되고
// - 원래 정해 졌던 스택의 범위를 넘을 정도로 함수가 중첩 호출이 되면 크래쉬가 나는 현상
// 예제로 팩토리얼을 함수로 구현해보겠습니다.

int Factorial(int n)
{	
	if (n <= 1)
		return 1;

	return n * Factorial(n - 1);
}
#pragma endregion
int main()
{	//오버로딩 예제 
	cout << Add(3, 5) << endl;
	cout << Add(1.5f, 2.1f) << endl;

	// 기본인자값 예제
	SetPlayerInfo(100, 40, 10);  // 네번째 인자로 길드아이디 값을 주지 않아도 됩니다.
	SetPlayerInfo(120, 30, 10, 0, 1);  // 다섯번째 인자에 값을 주기위해서는 생략됐던 기본인자도 기입해야합니다.

	// 스택 오버플로우 예제
	int fac = Factorial(5);		// 팩토리얼의 인자로 1000000 같은 일정 이상의 숫자를 넣어주면 크래쉬가 나면서 스택오버플로우라고 알림이 뜹니다.
	cout << "팩토리얼 : " << fac << endl;
}