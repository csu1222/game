#include <iostream>
using namespace std;

// 오늘의 주제 : 캐스팅 (타입 변환)

/*
1) static_cast
2) dynamic_cast
3) const_cast
4) reinterpret_cast

1 - static_cast : 타입 원칙에 비춰볼 때 상식적인 캐스팅만 허용해준다
	1. int <-> float
	2. Player* -> Knight* (다운캐스팅)  단, 안정성 보장 못함 (프로그래머 스스로 부모 자식간 캐스팅이 안전하다 확신한 후에 사용)
					(클래스의 멤버 변수에 타입 값이 있어서 조건문으로 원본이 뭔지 체크 후 사용 할 수도 있습니다.)

2 - dynamic_cast : 상속 관계에서의 안전한 형 변환
	RTTI(RunTime Type Information)활용
	 ↳ 다형성을 활용하는 방식 (virtual 함수가 하나라도 있어야합니다)
	  - virtual 함수를 하나라도 만들면, 객체의 메모리에 가상 함수 테이블(vftable) 주소가 기입된다.
	  - 만약 잘못된 타입으로 캐스팅을 했으면 nullptr을 반환 *******
		이를 이용해서 맞는 타입으로 캐스팅을 했는지 확인에 유용합니다.

3 - const_cast : const를 붙이거나 때거나~
	 만약 외부 라이브러리에서 가져온 함수가 char* 를 인자로 받을 때
	 문자열을 인자로 주면 문자열은 const char* 타입이라서 호환이 안될수 있습니다. 
	 그럴때 const를 때서 인자로 사용하기 위해 const_cast를 사용합니다.

4 - reinterpret_cast : 가장 위험하고 가장 강력한 형태의 캐스팅
	're - interpret' : 다시 - 간주하다/생각하다
	- 포인터랑 전현 관계없는 다른 타입 변환 등
	static_cast나 dynamic_cast의 경우 변환하는 타입간에 상관관계가 있어야 통과 시켜주었지만 
	reinterpret_cast는 프로그래머에게 모든 책임을 전가해서 어떤 타입으로든 캐스팅 할 수 있게 해주었습니다.
	malloc으로 할당된 메모리는 void*타입인데 이 메모리를 용도에 맞게 캐스팅 할 경우에 사용할 수 있습니다.
*/

class Player
{
public:
	virtual ~Player() { }

};

class Knight : public Player
{
public:
};

class Archer : public Player
{
public:
};

class Dog
{
public:
};

// const_cast 예시 함수
void PrintName(char* str)
{
	cout << str << endl;
}
int main()
{
	// static_cast 예시
	int hp = 100;
	int maxHp = 200;
	float retio = static_cast<float>(hp) / maxHp;
	
	// 부모 -> 자식  자식 -> 부모
	Player* p1 = new Knight();
	Knight* k1 = static_cast<Knight*>(p1);

	// dynamic_cast 예시
	Knight* k2 = dynamic_cast<Knight*>(p1);

	
	// const_cast 예시
	// 만약 외부 라이브러리에서 가져온 함수가 char* 를 인자로 받을 때
	// 문자열을 인자로 주면 문자열은 const char* 타입이라서 호환이 안될수 있습니다. 
	// 그럴때 const를 때서 인자로 사용하기 위해 const_cast를 사용합니다.
	PrintName(const_cast<char*>("tomas.k"));

	
	// reinterpret_cast 예시
	// 위의 k2라는 포인터의 주소를 __int64타입 변수에 저장하고 싶을 때 
	// k2는 __int64와 전혀 상관없는 타입이었지만 강제로 캐스팅 할 수 있습니다.
	__int64 address = reinterpret_cast<__int64>(k2);

	// 물론 정말 아무런 상관없는 클래스로도 강제 캐스팅을 할 수 있습니다.
	Dog* dog = reinterpret_cast<Dog*>(k2);
	// malloc의 경우 void* 를 반환하는데 이 메모리를 용도에 맞게 캐스팅하는등에 쓰일 수 있습니다.
	void* p = malloc(20);
	Dog* dog2 = reinterpret_cast<Dog*>(p);




	return 0;
}