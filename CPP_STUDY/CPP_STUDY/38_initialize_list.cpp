#include <iostream>
using namespace std;

// 오늘의 주제 : 초기화 리스트

/*
 멤버 변수 초기화 ? 다양한 문법이 존재

 초기화 왜 해야할까? 귀찮다~
  - 버그 예방에 중요
  - 포인터 등 주소값이 연루되어 있을 경우

  초기화 방법 
   - 생성자 내에서
   - 초기화 리스트
   - C++11 문법 ( 새로운 문법이라서 모든 팀에서 쓰는것이 아닌 팀 바이 팀)

   생성자 내에서 : 클래스의 생성자 함수 내에서 멤버변수들을 초기화 하는 방법
   초기화 리스트 : 클래스 상속할때 선처리 영역에서 생성자 함수를 고를 수 있었는데 그 공간에서 멤버 변수도 초기화 하는 것

   생성자 내에서 초기화 vs 초기화 리스트
    - 1) 일반 변수는 별 차이 없음
    - 2) 멤버 타입이 클래스인 경우 차이가 난다
	-- 멤버 타입이 클래스? : 프로그램이 복잡해지다보면 클래스내의 멤버 변수가 클래스일 수 있습니다.
	-- 클래스타입 멤버 변수를 사용할 때 어떤 방식으로 사용할지 생각해야 합니다.
	--- is-a 방식인지 has-a 방식인지
	--- is-a : Knight is-a Player? 라는 질문을 던져보는것  기사는 플레이어다 ) OK -> 상속관계
	---      - Knight is-a Inventory?  기사는 인벤토리냐?  NO -> 상속관계 X
	--- has-a : Knight has-a Inventory?  기사는 인벤토리를 포함하고 있냐?) OK -> 포함관계
	-- 클래스 타입 멤버 변수를 사용하면 객체를 생성할때 멤버 변수를 같이 생성해야 하니까 클래스 타입 멤버변수도 상속받은 부모 클래스 처럼 선처리영역에서 먼저 만들어집니다.
	-- 그래서 클래스 타입 멤버 변수를 초기화 할 때는 초기화 리스트 방법을 사용해 선처리영역에서 한번에 처리하는것이 좋겠습니다.
	- 3) 정의함과 동시에 초기화가 필요한 경우 (참조 타입, const 타입)
	-- 참조는 포인터와 비슷한 개념이지만 선언할때 초기화가 되어야 했습니다.
	-- const의경우 선언할 당시에 초기화 하지않으면 외부에서 값을 변경 할 수 없으니 초기화가 필요합니다.
	-- 그래서 참조, const 같은 타입의 멤버 변수가 필요하면 객체를 생성할때 선처리영역에서 처음부터 초기화를 하면서 생성해야 합니다.
  */
 
class Inventory
{
public:
	Inventory() { cout << "Inventory()" << endl; }
	Inventory(int size) { cout << "Inventory(int size)" << endl; _size = size; }

	~Inventory() { cout << "~Inventory()" << endl; }
	
public:
	int _size = 10;
};
class Player
{
public:
	Player() { }
	Player(int id) { }
};

class Knight : public Player
{
public:
	Knight() : Player(1), _hp(100) , _inventory(20), _hpRef(_hp), _hpConst(100) // 초기화 리스트
	/*
	선처리 영역

	_inventory = Inventory();

	*/
	{
		_hp = 100;  // 생성자에서 초기화
		// _inventory = Inventory(20);     생성자에서 초기화를 하였습니다.
		// 프로그램을 실행해보면 객체를 생성할때 선처리 영역에서 한번 이후 자신의 생성자 함수를 실행할때 다시 한번 Inventory객체를 불러오고 있습니다.
	}

public:
	int _hp;
	Inventory _inventory;

	// 문법 설명용
	int* _hpPtr = &_hp;  // 포인터의 경우 초기화 없이 선언 할 수 있었습니다 (nullptr)
	int& _hpRef;  // 참조는 포인터와 비슷한 개념이지만 선언할때 초기화가 되어야 했습니다.
	const int _hpConst;  // const의경우 선언할 당시에 초기화 하지않으면 외부에서 값을 변경 할 수 없으니 초기화가 필요합니다.
};



int main()
{

	Knight k;

	cout << k._hp << endl;


	return 0;
}