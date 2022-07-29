#include <iostream>
using namespace std;

// 오늘의 주제 : 생성자와 소멸자

// 생성자와 소멸자는 class와 관련된 문법입니다.

/*
  [생성자(Constructor)와 소멸자(Destructor)]
  클래스에 '소속'된 함수를 멤버 함수라고 합니다.
  이 중에서 굉장히 특별한 함수 2종이 있는데, 바로 [시작]과 [끝]을 알리는 함수들
  - 시작(탄생) -> 생성자 (여러개 존재 가능)
  - 끝(소멸) -> 소멸자 (오직 1개만)
 */

/*
1) 인자없을 때
 [암시적(Implicit) 생성자]
 생성자를 명시적으로 만들지 않으면
 아무 인자도 받지 않는 [기본생성자]가 컴파일러에 의해 자동으로 만들어짐
 --> 그러나 명시적(Explicit)으로 아무 생성자를 하나 만들면,
 자동으로 만들어지던 [기본 생성자]는 더 이상 만들어 지지 않음!

 2) 자기의 클래스 타입을 인자로 받을 때
 복사 생성자를 명시적(Explicit)으로 만들지 않아도
 암시적(implicit)으로 복사생성자를 컴파일러가 만들어서 똑같은 멤버 변수값을 가진
 객체를 생성합니다.
*/



/* 이전시간 보충
 시작전에 표기법에 대해 조금 이야기 하자면
 멤버 함수의 경우 첫글자를 대문자로 표시하는 것이 대부분이고
 멤버 변수의 경우 함수의 지역변수인지 멤버 변수인지 햇갈리지 않게 하기 위해
 m_변수이름, m대문자변수이름, _변수이름 등 각 팀마다 다른 스타일로 멤버 변수를 표시해줍니다.

 this : 클래스내의 멤버변수를 멤버 함수에 사용할때 메모리상에서 멤버 변수의 주소를 'this'라는 바구니에 넣어 사용했습니다.
 - 그래서 멤버 함수 안에 this; 를 입력하면 해당 클래스 타입의 포인터라고 설명이 되어 있습니다.
 예시) Knight::Die 함수에서 _hp = 0; 과 this->_hp = 1; 을 어셈블리 상에서 까보면 동일한 방식으로 동작 한다는 것을 알 수 있습니다.
*/


class Knight
{
public:					
	// [1] 기본 생성자 (인자 없음)
	// 객체를 생성할때 무조건 한번 호출이 됩니다.
	Knight()
	{
		cout << "Knight 기본 생성자 호출" << endl;

		// 생성자 함수로 멤버 변수들을 초기화 해주는 용도도 있을 것입니다.
		_hp = 100;
		_attack = 10;
		_posY = 0;
		_posX = 0;
	}

	// [2] 복사 생성자 (자기 자신의 클래스 참조타입을 인자로 받음)
	// - 일반적으로 '똑같은' 데이터를 가진 객체가 생성되길 기대한다
	// - 주 목적이 '복사'이므로 인자에 대부분 const를 붙인다.
	Knight(const Knight& knight)
	{
		_hp = knight._hp;
		_attack = knight._attack;
		_posY = knight._posY;
		_posX = knight._posX;
	}

	// [3] 기타 생성자

	// 이 중에서 인자를 1개만 받는 [기타 생성자]를 
	// [타입 변환 생성자]라고 부르기도 함
	Knight(int hp)		// 인자가 필요할 경우 상수가 아닌 인자값을 초기화 해줍니다.
	{
		cout << "Knight(int) 생성자 호출" << endl;
		_hp = hp;
		_attack = 10;
		_posY = 0;
		_posX = 0;
	}
	// [타입 번환 생성자]를 명시적으로 사용할 것!
	/*explicit Knight(int hp)
	{
		cout << "Knight(int) 생성자 호출" << endl;
		_hp = hp;
		_attack = 10;
		_posY = 0;
		_posX = 0;
	}*/

	// 인자가 여러개인 [기타생성자]
	Knight(int hp, int attack, int posY, int posX)
	{
		_hp = hp;
		_attack = attack;
		_posY = posY;
		_posX = posX;

	}


	// 소멸자
	~Knight()
	{
		cout << "Knight 기본 소멸자 호출" << endl;
	}


	void Move(int y, int x);
	void Attack();
	void Die()
	{
		_hp = 0;			
		cout << "Die" << endl;

	}

public:
	int _hp;
	int _attack;
	int _posX;
	int _posY;
};


void Knight::Move(int y, int x)
{
	_posY = y;
	_posX = x;
	cout << "Move" << endl;
}

void Knight::Attack()
{
	cout << "Attack : " << _attack << endl;
}

// 타입변환 생성자가 암시적으로 동작할 함수
void HelloKnight(Knight knight)
{
	// 인자로 Knight클래스를 받긴 했지만 사용하지 않겠습니다.
	cout << "Hello Knight!" << endl;
}

int main()
{
	Knight k1;
	k1._hp = 100;
	k1._attack = 10;
	k1._posY = 0;
	k1._posX = 0;

	k1.Move(2, 2);
	k1.Attack();
	k1.Die();

	//인자가 있는 경우
	Knight k2(100);

	// 복사생성자
	Knight k3(k1);		
	Knight k4 = k1;		// 이런 형태도 복사생성자로 인식됩니다.
	
	// 하지만 밑과 같이 복사하면 일단 기본생성자로 빈 객체를 생성하고 난 뒤 멤버 변수값을 복사해주는 것입니다.
	Knight k5;
	k5 = k1;

	//k1._hp = 100;
	k1._attack = 10;
	k1._posY = 0;
	k1._posX = 0;


	// 암시적 형변환 --> 컴파일러가 알아서 바꿔치기
	int num = 1;

	float f = (float)num;  // 명시적 < 우리가 코드로 num을 float 바구니에 넣으라고 하고있음
	double d = num;			// 암시적 < 별 말안했는데 컴파일러가 알아서 처리하고 있음
	
	// 아무리 암시적으로 형변환 해준다지만 뭔가 이상합니다.
	// k6 = 1; 이라는 코드를 어셈블리 상에서 따라가 보면 인자를 한개만 받는 생성자(타입 변환 생성자)로 call된다는걸 알 수 있습니다.
	// Knight k6(1); 이라는 코드와 결과가 같습니다.
	Knight k6;
	k6 = 1;	

	// 인자로 클래스를 받는 함수가 있다고 가정 했을 때 
	HelloKnight(5);
	// 인자로 정수가 들어갔지만 함수가 기능 합니다. 인수로 들어간 5는 타입변환 생성자에 의해 의도하지 않은 객체가 생성되었습니다.
	// explicit [타입 변환 생성자] 로 의도하지않은 객체 생성을 방지 할 수 있습니다.


	return 0;
}