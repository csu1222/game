#include <iostream>
using namespace std;

// 오늘의 주제 : 상속성 (Inheritance)

// 객체지향 (OOP Object Oriented Programming)
// - 상속성  * 
// - 은닉성 
// - 다형성



struct StatInfo
{
	int hp;
	int attack;
	int defence;
};



// 메모리는
//  [    [ Player ]   ]
//  [     Archer      ]

// 상속 (Inheritance) ? 부모 -> 자식에게 유산을 물려주는것

// 생성자(N)/소멸자(1)
// - 생성자는 탄생을 기념해서 호출되는 함수?
// -- Archer를 생성하면 -> Player의 생성자? Archer의 생성자?
// -- 솔로몬의 선택! 그냥 둘다 호출하자!
// -- 그러면 생성자/소멸자 호출순서? : 부모생성자, 자식생성자, 자식소멸자, 부모소멸자 순으로 생성된다고는 하는데
// -- 사실 어셈블리에서 보면 그렇지 않았습니다.객체가 생성되면 우선 생성된 자식 생성자로 호출이되고 
// -- 자식생성자의 선처리 영역(구현부분{}을 실행하기전) 에 다시 부모 생성자로 호출이 되었습니다.
// -- 소멸자의 경우에는 이 프로그램이 실행이 끝나고 난뒤 스택을 정리하면서 Archer소멸자가 호출이됩니다.
// -- Atrcher소멸자가 실행되고난 직후 후처리영역(함수가 끝나고 다음 프로세스로 넘어가기 직전 영역) 에서 
// -- 부모소멸자를 호출하여 실행하고 다시 자식 소멸자로 돌아온뒤 main의 return이후로 진행이 됩니다.
// - 부모,자식 생성자가 인자를 받는등 다른 생성자를 가질때 
// -- 객체를 생성할때 인자를 넣으면 기본적으로 부모생성자는 기본 생성자, 자식은 인자를 받는 생성자가 호출됩니다.
// -- 부모도 인자를 받은 생성자를 호출 하고 싶으면 자식생성자(인자를 받는) 옆에 : 부모생성자(인자) 의 형태로 작성해주면됩니다.




// 공통된 분모를 가지는 class Player를 만들었습니다.
class Player
{
public:
	Player()
	{
		_hp = 0;
		_attack = 0;
		_defence = 0;
		cout << "Player() 기본 생성자 호출" << endl;
	}
	// 부모 생성자가 하나가 아닐수 있습니다.
	Player(int hp)
	{
		_hp = hp;
		_attack = 0;
		_defence = 0;
		cout << "Player(hp) 생성자 호출" << endl;
	}

	~Player()
	{
		cout << "Player() 소멸자 호출" << endl;
	}

	void Move() { cout << "Player Move 호출" << endl; }
	void Attack() { cout << "Player Attack 호출" << endl; }
	void Die() { cout << "Player Die 호출" << endl; }

public:
	int _hp;
	int _attack;
	int _defence;
};

// class 는 일종의 설계도
// 게임에 하나의 직업만 있는게 아니고 곂치는 속성, 기능들이 있을 것입니다.
// 속성의 경우 이전에 배우 struct문법으로 묶어줄 수 있을 것입니다.
// 하지만 멤버함수는 그저 함수이니까 지금까지 배운바로는 별 수가 없었습니다.
// 이런 고민을 해결 할 수 없을까에서 나온것이 '상속'의 개념입니다.
class Knight
{
public:
	void Move() {}
	void Attack() {}
	void Die() {}
public:
	StatInfo _statInfo;
};

class Mage
{
public:
	void Move() {}
	void Attack() {}
	void Die() {}
public:
	StatInfo _statInfo;
};

// 상속 문법 : 부모 클래스의 속성, 기능들을 물려받는것
// Archer 클래스는 텅 비어 있지만 부모 클래스의 속성들을 모두 물려 받았습니다.
class Archer : public Player
{
public:
	Archer()
	{
		_agility = 50;
		cout << "Archer() 기본 생성자 호출" << endl;
	}
	Archer(int ahility) : Player(100) // : Player(100) 부분은 부모생성자의 인자를 받는 생성자를 따로 호출할 때 사용
	{
		_agility = ahility;
		cout << "Archer(agility) 생성자 호출" << endl;
	}
	~Archer()
	{
		cout << "Archer() 소멸자 호출" << endl;
	}

	// 재정의 : 부모님의 유산을 거부하고 새로운 이름으로 만든?
	void Move() { cout << "Archer Move 호출" << endl; }

public:
	// Archer 만의 속성을 추가 해보겠습니다.
	int _agility;

};

int main()
{
	Archer A;

	
	A._hp = 100;
	A._attack = 10;
	A._defence = 5;
	A._agility = 50;

	// 인자를 받는 생성자를 자식 부모 둘다 가지고 있을때
	Archer Robin(100);

	//A.Attack();
	//A.Die();
	//A.Move();
	//A.Player::Move();  // 재정의 했던 부모클래스의 원래 속성을 가져오는법


	return 0;
}