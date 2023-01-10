#include <iostream>
using namespace std;

// 오늘의 주제 : 다중 포인터

// 복습 해보면서 다중 포인터에 대해 알아 보겠습니다.

void SetNumber(int* a)
{
	*a = 1;
}

void SetMessage(const char* a)
{
	a = "Bye";

	// *a = 'B';  *로 값에 접근해도 const char 형 이기 때문에 수정 할수 없었습니다.
	// 그래도 const가 없다면 수정이 가능하고
	// *a = 'B';  나  *(a + 1) = 'y';  또는 a[2] = 'e'; 로 수정 할 수 있었습니다.
}


// 다중 포인터
void SetMessage(const char** a)
{
	*a = "Bye";
}

// 다중포인터 참조도 가능한가?
void SetMessage2(const char*& a)
{
	a = "apple";
}

int main()
{
	// 복습) 포인터는 주소를 담는 바구니 라고 했습니다. 
	int a = 0;
	SetNumber(&a);
	cout << a << endl;


	// const char* 형태
	const char* msg = "Hello";
	SetMessage(msg);
	cout << msg << endl; // Hello? Bye? 
	
	
	// 그러면 매개변수로 int* 을 줘서 값을 고치던 것 처럼 const char* 을 고치려면?
	// const char* 에 int* 처럼 *을 붙혀주면 될까? 하지만 아쉽게도 이미 *이 붙어있습니다.
	// 하지만 놀랍게도 그게 정답입니다.
	// 다중 포인터

	// .rdata Hello주소[H][e][l][l][o][\0]
	// 주소1[ Hello주소 ] << 8바이트
	// pp[ &msg ] << 8바이트
	const char** pp = &msg;

	// [매개변수]RET][지역변수]  [매개변수 a(&msg msg의 주소)]RET][지역변수]
	SetMessage(pp);
	cout << msg << endl;

	// 이차 포인터 뿐만 아니라 삼차 사차 까지 같은 방식으로 *을 늘릴 수는 있습니다.
	// 하지만 실질적으로 이차 이상은 의미가 없다고 합니다.

	// 지금까지의 다중 포인터의 의미!
	// 일반 지역변수를 함수의 매개변수로 삼기 위해 포인터를 사용 했었습니다.
	// 하지만 문자열같은 이미 포인터형 변수를 함수의 매개변수로 사용하면 문자열의 원본을 건드려야 하는데 문자열은 
	// 읽기전용 데이터이므로 수정할 수 없었습니다. 
	// 그래서 문자열에 다시 포인터를 달아 함수에서 매개변수로 사용해도 문자열 지역변수를 수정할 수 있으면서도
	// 문자열 지역변수의 값을 다시 초기화 하면서 값을 수정할 수 있게 됩니다.

	// 다중 포인터 :  혼동스럽다?
	// 그냥 양파까기라고 생각하면 된다.
	// *을 하나씩 까면서 타고 간다고 생각

	// 번외) 그러면 참조도 가능한가? 가능!
	SetMessage2(msg);
	cout << msg << endl;

	return 0;
}