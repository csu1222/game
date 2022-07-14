#include <iostream>
using namespace std;

// 오늘의 주제 : 데이터의 연산
// 데이터를 가공하는 방법에 대해 알아보겠습니다.

// a[ 1 ] 
// a라는 이름의 바구니를 할당하고 안에 1을 넣는다.
int a = 1;

// b[ 2 ]
// b라는 이름의 바구니를 할당하고 안에 2를 넣는다.
int b = 2;

int hp = 100;
bool isInvincible = true;

bool isSame;
bool isDifferent;
bool isGreater;
bool isSmaller;

bool test;

int main()
{
#pragma region 산술연산자
	// 산술 연산자
	
	
	// 대입 연산자
	// a에 b를 대입하고 b를 반환하라.
	// -> b 라는 바구니 안에 있는 값을 a 라는 바구니 안에 복사해라.
	a = b;


	// 사칙 연산자
	// 언제 필요한가?
	// ex) 데미지 계산
	// ex) 체력을 깍는다거나
	// ex) 루프문에서 카운터를 1 증가 시킨다거나

	a = b + 3;  // 덧셈 add
	a = b - 3;  // 뺄셈 sub
	a = b * 3;  // 곱셈 mul
	a = b / 3;  // 나눗셈 div
	a = b % 3;  // 나머지 div

	a += 3;  // a = a + 3; 과 같음
	a -= 3;
	a *= 3;
	a /= 3;
	a %= 3;

	// 증감 연산자
	a = a + 1;  // 어셈블리에서  add eax, 1   =   inc eax
	a++;  
	++a;
	a--;
	--a;

	b = a++;  // b = a; -> a를 1증가
	b = ++a; //  a를 1증가 -> b = a;

	b = (a + 1) * 3;  // 수학과 마찬가지로 곱셈이 우선하므로 괄호를 적절히 사용해야합니다.

#pragma endregion

#pragma region	비교연산

	// 언제 필요한가? 
	// ex) 체력이 0이 되면 사망처리
	// ex) 체력이 30% 이하가 되면 궁극기를 발동 (100 * hp / maxhp)
	// ex) 경험치가 100 이상이면 레벨업

	// a == b;	: a 와 b 의 값이 같은가?
	// 같으면 1, 다르면 0
	isSame = (a == b);

	// a != b;  : a와 b가 다른가?
	isDifferent = (a != b);

	// a > b : a가 b보다 큰가?
	// a >= b : a가  b보다 크거나 같은가?
	isGreater = (a > b);

	// a < b
	// a <= b
	isSmaller = (a < b);
#pragma endregion

#pragma region 논리연산
	// 언제 필요할까? 조건에 대한 논리적 사고가 필요할 때
	// ex) 로그인 할 때 아이디도 같고 비밀번호도 같아야 한다.
	// ex) 길드 마스터 OR 길드 운영자 이면 길드 해산이 가능.

	// ! not
	// 0 이면 1을 반환, 그 외는 0을 반환
	test = !isSame;  // 사실상 isDifferent 의 의미

	// &&	and
	// a && b -> 둘다 1이면 1, 그 외에는 0
	test = (hp <= 0 && isInvincible == false); // 죽음

	// ||	or
	// a || b -> 둘 중 하나라도 1이면 1 (둘다 0이면 0)
	test = (hp > 0 || isInvincible == true);

#pragma endregion
}