#include <iostream>
using namespace std;

// 오늘의 주제 : 은닉성(Data hiding)

// 객체지향 (OOP Object Oriented Programming)
// - 상속성   
// - 은닉성  
// - 다형성  *

/*
 다형성 (Polymorphism  =  Poly + morph) = 겉은 똑같은데 기능이 다르게 동작한다
 - 오버로딩(Overloading) = 함수를 중복 정의 = 함수의 이름을 재사용
 - 오버라이딩(Overriding) = 재정의 = 부모클래스의 함수를 자식 클래스에서 재정의
 -- 함수 MovePlayer(Player* player); 로 부모객체와 자식객체를 모두 아우를 수는 있었지만
 -- 자식 함수에서 MovePlayer실행시 부모클래스의 Move 함수라 실행되었습니다. 그러면 오버라이딩 하는 의미가 없을 것입니다.
 -- 부모,자식 함수에 가상함수가 있으니 일반 함수에서도 인자로 받은 클래스의 함수가 실행되었습니다.
 -- MovePlayer(Player* player); 함수는 어떤 인자가 들어오더라도 정의될때의 Player* 의 멤버함수만 실행된다고 했는데
 -- 가상함수를 사용하니 인자로 받은 Knight의 멤버함수가 실행되었습니다.
 -- 먼저 MovePlayer 함수는 Playre포인터를 인자로 받는데 Knight주소가 인자로 가능했던것은 Knight클래스 자체가 Player클래스를 상속 받고 있기 때문입니다.
 -- 


 바인딩(Binding) = 묶는다
 - 정적 바인딩(Static Binding) : 컴파일 시점에 결정
 - 동적 바인딩(Dynamic Binding) * 중요 : 실행 시점에 결정
 
 일반 함수는 정적 바인딩을 기본적으로 사용 
 동적 바인딩을 원한다면? -> 가상 함수(virtual function)  
 가상함수의 형태는
 virtual [반환타입] [함수이름] [구현부]

 그런데 실제 객체가 어떤 타입인지 어떻게 알고 알아서 가상함수를 호출해준걸까?
 - 가상 함수 테이블 (vftable
 .vftable [ ] 4바이트(32비트) 8바이트(64비트) 의 주소 
 [VMove] [VDie] 

 순수 가상 함수 : 구현은 없고 '인터페이스'만 전달하는 용도로 사용하고 싶을 경우
  - 상위 클래스에 구현이 없고 실제 사용되는 하위 클래스에서 함수 기능을 구현해 사용하라는 의미
  - 추상 클래스 : 순수 가상 함수가 1개 이상 포함되면 바로 추상 클래스로 간주
    -- 직접적으로 객체를 만들 수 없게 됨

 */


class Player
{
public:
	Player()
	{
		_hp = 100;
	}
	void Move() { cout << "Move Player!" << endl; }
	// 오버로딩 - 받는 인자의 갯수나 타입이 다르면 이름을 재사용 할 수 있다
	//void Move(int a) { cout << "Move Player (int)" << endl; }
	virtual void VMove() { cout << "VMove Player!" << endl; }
	virtual void VDie() { cout << "VDie Player!" << endl; }

	virtual void VAttack() = 0; // 순수 가상 함수 문법

public:
	int _hp;
};

class Knight : public Player
{
public:
	Knight()
	{
		_stamina = 100;
	}
	void Move() { cout << "Move Knight!" << endl; }

	// 가상함수는 재정의를 하더라도 가상함수다! (재정의 할 때 virtual을 없애도 가상함수)
	virtual void VMove() { cout << "VMove Knight!" << endl; }
	virtual void VDie() { cout << "VDie Knight!" << endl; }
	virtual void VAttack() { cout << "VAttack Knight" << endl; }

public:
	int _stamina;
};

class Mage : public Player
{
public:
	void Move() { cout << "Move Mage!" << endl; }
	virtual void VMove() { cout << "VMove Mage!" << endl; }
	virtual void VDie() { cout << "VDie Mage!" << endl; }
	virtual void VAttack() { cout << "VAttack Mage" << endl; }

public:
	int _mp;
};
void MovePlayer(Player* player)  // * 공통된 부모를 다루는 함수를 만들면 하위 객체마다 함수를 만들지 않아도 되겠습니다.
{
	player->VMove();
	player->VDie();
}

void MoveKnight(Knight* knight)
{
	knight->VMove();
}
int main()
{
	//Player p;		// 순수 가상 함수가 포함되어 추상 클래스가되서 독단적으로 객체를 생성할 수 없어졌습니다.
	//p.Move();
	//MovePlayer(&p);  // 플레이어는 플레이어다? YES
	// MoveKnight(&p);	   // 플레이어는 기사다? NO

	Knight k;
	k.Move();
	MoveKnight(&k);   // 기사는 기사다? YES
	MovePlayer(&k);   // 기사는 플레이어다? YESㄴ

	return 0;
}