#include <iostream>
using namespace std;

// 한번 정해지면 절대 바뀌지 않을 값들
// constant의 약자인 const를 붙임 (변수를 상수화 한다고 함)
// const를 붙였으면 초기값을 반드시 지정해야 합니다.

unsigned char flag;

int AIR = 0;
int STUN = 1;
int POLYMORPH = 2;
int INVINCIBLE = 3;


// 변수를 사용하면서 사용했던 변수명을 다시 조작하는 실수가 있을수 있습니다.
// INVINCIBLE = 8;
// 이렇게 값을 조작하는게 의도했던 것이면 상관 없지만 
// 의도없이 실수로 조작할 경우 문제가 생길 것입니다.
// 이런 실수가 생기지 않을 최소한의 조치로 위에서 배운 const를 이용합니다.


const int AIR2 = 0;
const int STUN2 = 1;
const int POLYMORPH2 = 2;
const int INVINCIBLE2 = 3;

// const를 사용하면 밑 처럼 다시 실수를 해도 에러가 나게되므로 스노우 볼을 예방할 수 있습니다.
//INVINCIBLE2 = 5;

// 프로젝트 중간에 변수들에 추가나 제거같은 수정시에 더욱 용이합니다.
/*
const int AIR2 = 0;
const int STUN2 = 1;
const int FEAR2 = 2;
const int POLYMORPH2 = 3;
const int INVINCIBLE2 = 4;
*/

// 전역변수 
// - 함수 밖에서 선언된 변수

// [데이터 영역]
// .data 영역 (초기값이 있는 경우)
int a = 2;
// .bss 영역 (초기값이 없는 경우)
int b;
// .rodata (읽기 전용 데이터)
const char* c = "Hello World";


// 전역변수, 스택영역의 중요서은 
// c++ 이라는 언어가 권한이 강력한 만큼 위험한 언어이기 때문에 
// 어디서 어떤 에러가 일어났는지 알아야 하기 때문

// 그러면 const 변수는 바뀌지 않는 변수이니까
// .rodata 영역에 있어야하나?
// 사실 C++ 표준에서는 꼭 그러라는 말은 없습니다.
// 그냥 컴파일러 (여기서는 visual studio) 마음이라는 것

int main()
{	
	// [스택 영역]
	int c = 3;
	// 함수 내에 정의한 변수는 본 함수에서만 사용가능합니다.


#pragma region 비트연산

	// 무적상태를 변수로 표현	 변수로 표현하면 가독성이 좋아집니다. 
	flag = (1 << INVINCIBLE);  

	// 변이 상태를 추가한다면(무적 + 변이)
	flag |= (1 << POLYMORPH);  


#pragma endregion
}

// 유의사항 
#pragma region 변수의 유효범위
/*	변수의 유효범위
	전역변수
	 - 함수 밖에서 선언된 전역 변수는 어디에서도 호출 할 수 있습니다.
	int hp = 10;

	스택
	 - 함수 안에서 정의된 변수는 스택 영역에 위치하고 
	 - 선언된 { } 안이 생존범위입니다.

	 ex) 
	 int main()
	 {
		int hp = 10;
	 }
	 int test()
	 {
		hp = 50; // 처음 정의된 main함수 밖의 다른 함수에서 hp 변수를 호출하면 error가 납니다.
	 }

	 ex2)
	 int main()
	 {
		{
		int hp = 50;		이 변수는 함수안에서 다시 {} 안에 선언 되었기때문에 
		}

		int hp = 30;		여기에 다시 같은 이름으로 선언 할 수 있습니다
	 }
*/
#pragma endregion

#pragma region 연산 우선순위
/*
	
	예시로 짝수 여부 판단
	int hp = 20;

	bool isEven = (hp % 2 ==0);
	위의 경우 괄호 안에 % 나머지 연산자가 우선 하는지 == 동등비교연산자가 우선인지 했갈립니다.
	- 검색어 : C++ 연산 우선도

	bool isEven = ((hp % 2) == 0);
	이렇게 의도한 순서대로 동작하도록 괄호를 사용하면 가독성도 오릅니다.

	// 짝수거나 3으로 나뉘는 값인지
	bool isEvenOrDivBy3 = (hp % 2 == 0 || hp % 3 == 0);

	bool isEvenOrDivBy3 = (((hp % 2) == 0) || ((hp % 3) == 0));
	
*/
#pragma endregion

#pragma region 타입변환
int 타입변환() 
{
	int hp = 20;

	cout << hp << endl;

	// 바구니 교체

	short hp2 = hp;		// 윗쪽 비트 데이터가 짤린 상태로 저장
	float hp3 = hp;		// 실수로 변환 할 때는 정밀도 차이가 있기 때문에 데이터 손실
	unsigned int hp4 = hp;  // 비트 단위로는 똑같은데, 분석하는 방법이 달라집니다.

	// 변환에는 
	// short hp2 = (short)hp;
	// 이런 식으로 괄호 부분이 생략 되어 있는 것입니다.

}

#pragma endregion

#pragma region 사칙 연산 관련
int calculator()
{
	// 곱셈
	// - 오버플로우
	// int* int = int;
	// 인트와 인트 끼리 곱해서 값이 인트가 나오지만 각 변수값이 너무 크다면 인트의 범위 안에 다 못담을 수 있습니다.
	// __int64 로 먼저 변환해서 오버플로우를 예방할 수 있습니다.

	// 나눗셈
	// - 0 으로 나누기 조심
	int hp = 123;
	int maxHp = 1000; // 원래 값은 1000이지만 오타로 0일경우 밑의 연산에서 크래쉬가 납니다.
	float ratio = hp / maxHp;
	// - 실수 관련
	float ratio2 = hp / maxHp;  // 값이 0.0000으로 나옵니다.
	// int와 int 끼리 나눗셈 연산에서는 값도 int로 나오므로 소숫점 자리가 버려집니다.
	// 둘 중 하나라도 float 타입이면 결과가 float가 나옵니다.
	float ratio3 = hp / (float)maxHp;
}
#pragma endregion