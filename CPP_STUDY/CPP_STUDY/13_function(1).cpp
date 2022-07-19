#include <iostream>
using namespace std;

// 오늘의 주제 : 함수 (프로시저, 메소드, 루틴)

/*
* 함수의 얼개
input으로 무엇을 받고 output으로 무엇을 뱉을지 정해준다

반환타입 함수이름(인자타입(매개변수))
{
	함수 내용

	return ~~
}

*/

// Hello World를 콘솔에 출력하는 함수를 만들어 보자
// input : 없음 / output : 없음
// 타입 : int, float, double, char~ , 반환하는게 없다 void

void PrintHelloWorld()
{
	cout << "Hello World" << endl;
}

// 정수를 입력(함수) 받아서, 콘솔에 출력하는 함수를 만들어 보자.
// input : int  // output : 없음
void PrintNumber(int number)
{
	cout << "넘겨주신 숫자는 " << number << " 입니다." << endl;
}

// 2를 곱하는 함수를 만들어보자
// input : int // output : int

int MultiplyBy2 (int a) 
{
	int b = a * 2;
	return b;
}

// 두 숫자를 받고 받은 두 숫자를 곱한 걸 뱉는 함수
// input : int, int // output : int

int MultiplyBy(int a, int b)
{
	int c = a * b;
	return c;
}

// 번외 : 디버그 단축키
// F5 : 중단점 까지 실행 (다음 중단점이 없다면 디버그 종료)
// F10: 프로시저 단위 실행 , 프로시저는 함수의 다른 이름입니다. 한줄 씩 디버그를 하다가 함수를 만나면 함수 안으로 까지는
// - 가지 않고 다음줄로 실행됩니다.
// F11: 한단계 씩 실행 한 줄 씩 디버그를 하다가 함수를 만나면 그 함수안에 까지 들어가 한줄씩 실행합니다.

int main()
{
	int result = MultiplyBy(3, 5);
	PrintHelloWorld();
	PrintNumber(result);
}