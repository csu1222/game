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

unsigned char flag;		// 플래그로써 사용할 때는 unsigned로 만들어서 최상위 비트를 자유롭게 사용할수 있습니다.

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

#pragma region 비트연산

	// 언제 필요한가? (사실 많이는 없음)
	// 비트 단위의 조작이 필요할 때
	// - 대표적으로 BitFlag

	// ~  bitwise not
	// 단일 숫자의 모든 비트를 대상으로, 0은 1, 1은 0으로 뒤바꿈
	// ex) 0100 0010 이라는 1바이트 숫자를 ~ 하면 1011 1101

	// &  bitwise and
	// 두 숫자의 모든 비트를 대상으로 and
	// ex) 1001 1010 과 1101 0011 을 & 하면 1001 0010 

	// |  bitwise or
	// 두 숫자의 모든 비트를 대상으로 or
	// ex) 1001 1010 과 1101 0011 을 | 하면 1101 1011

	// ^  bitwise xor
	// 두 숫자의 모든 비트를 대상으로 xor
	// xor는 두 대상을 비교해서 다르면 1, 같으면 0
	// xor의 특징으로 같은 숫자를 xor 하면 0으로 초기화해준다.
	// - 또 다른 특징으로 어떤 숫자와 xor를 두번 하면 자기 자신으로 돌아옵니다. (암호화에서 사용)

	int ax = 1;
	int bx = 123;
	a = a ^ b;
	a = a ^ b;

	// <<  bit shift Left
	// 비트열을 N만큼 왼쪽으로 이동한다.
	// 왼쪽에 넘치는 N개의 비트는 버린다.(복원되지않음) 그리고 새로 생성되는 오른쪽의 N개의 비트는 0.
	// *2 를 할 때 자주 보이는 패턴(bit 2진수의 특징)

	// >> bit shift Right
	// 비트열을 N만큼 오른쪽으로 이동한다.
	// 오른쪽에 넘치는 N개의 비트는 버린다
	// 왼쪽에 새로 생성되는 N개의 비트는
	// - 부호 비트가 존재할 경우 부호 비트를 따라감 (부호있는 정수라면 이 부분을 유의함
	// - 아니면 0  (unsigned 타입의 정수)

	// 실습 - 어떤 게임에서 각 비트마다 속성을 나타내게 했을 경우
	// 0b0000 [무적][변이][스턴][에어본]
	// 그냥 2진수,정수로 볼 수 도 있지만 각 비트에 의미를 부여여 해석하면 의미가 생긴다.
	
	// 무적상태를 BitFlag로 표현
	flag = (1 << 3); // = 0b1000 = 8   

	// 변이 상태를 추가한다면(무적 + 변이)
	flag |= (1 << 2);  // 0b1000 | 0b0100 = 0b1100 

	// 무적인지 확인하고 싶다.(다른 상태는 관심없다.)
	// bitmask
	bool invincible = ((flag & (1 << 3)) != 0);

	// 무적이거나 스턴 상태 인지 확인하고 싶다면?
	int mask = (1 << 3) | (1 << 1);
	bool stunOrInvicible = ((flag & mask) != 0);


#pragma endregion
}