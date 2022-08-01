#include <iostream>
using namespace std;

// 오늘의 주제 : 연산자 오버로딩 (Operator Overloading)

/*
 연산자 vs 함수
  - 연산자는 피연산자의 개수/타입이 고정되어 있음

 연산자 오버로딩?
 일단 [연산자 함수]를 정의해야 함
 함수도 멤버함수 vs 전역함수가 존재하는것 처럼, 연산자 함수도 두가지 방식으로 존재 할 수 있음.

 - 멤버 연산자 함수 version
  -- a op b 형태에서 왼쪽을 기준으로 실행됨 (a가 클래스여야 가능, a를 '기준 피연산자'라고 함)
  -- 한계) a가 클래스가 아니면 사용 못함

 - 전역 연산자 함수 version
 -- a op b 형태라면 a, b 모두 연산자 함수의 피연산자로 만들어준다

 멤버 연산자 함수 vs 전역 연산자 함수 
 - 전역 연산자 함수가 좀 더 전방위 적으로 사용 할수 있었으니깐 전역 연산자 함수만 사용하면 되겠네~ (NO)
 - 그럼 무엇이 더 좋은가? - 그런건 없음 , 심지어 둘 중 하나만 지원하는 경우도 있으니 둘다 사용 할 줄 알아야 합니다.
 -- 대표적으로 대입 연산자 (a = b)는 전역 연산자 함수로는 못 만든다
 --- 프로그래밍에서 대입 연산자라는 것 자체가 오른쪽의 값을 왼쪽의 바구니에 넣는다(복사한다?) 라는 의미인데 전역 연산자 함수를 허용한다면 함수내에서 왼쪽의 값을 오른쪽으로 넣어주는 위험한 일을 할 수 있기 때문에 문법적으로 막아 뒀다 라고 할 수 있습니다.
 디테일+ ) 대입연산자는 void 타입처럼 동작만 할 때도 있지만 b = (a = 3) 처럼 값으로써 사용 가능하기도 했습니다.
  - 함수의 output 타입을 자신의 참조 타입으로 해서 대입후 return *this; 로 반환도 할 수 있고 반환 없이 그저 대입만도 가능하게 한다고 합니다.

  복사 대입 연산자
  - 대입 연산자가 나온 김에 [복사 대입 연산자]에 대해 알아봅시다.
  - 용어가 했갈립니다. [복사 생성자], [대입 연산자], [복사 대입 연산자] @_@;
  - 복사 대입 연산자 = 대입 연산자 중, 자기 자신의 참조 타입을 인자로 받는 것
  -- [복사 대입 연산자]나 [복사 생성자]가 특별대우를 받는 이유는! 
  -- 말 그대로 객체가 '복사' 되길 바라는 특징 때문
  -- TODO ) '동적할당' 시간에 더 자세히 알아볼것 입니다.

  기타
  - 모든 연산자를 다 오버로딩 할수 있는것은 아닙니다.(::  .  .*  이런건 안됨)
  - 모든 연산자가 다 2개의 항이 있는 것은 아님 ++ , -- 가 대표적 (단항 연산자)
  증감 연산자 (++, --)
  - 전위형 (++a) operator++()
  - 후위형 (a++) operator++(int)
  -- 전위형 : c = ++a;  a를 증감연산 후 c에 대입합니다.
  -- 후위형 : c = a++;  c에 a를 대입한후 a를 증감합니다.
  - 전위형 증감연산자 : 증감연산후 대입이 가능하도록 자신클래스 참조 타입을 return 합니다. (Position& operator++())
  - 후위형 증갑연산자 : 증감연산이 되기전 값을 대입되도록 증감전 자신의 값을 임시 객체에 복사해야합니다. (Position operator++(int))
*/

class Position
{
public:
	Position operator+ (const Position& arg) // 멤버 함수 version 연산자 오버로딩
	{
		Position pos;
		pos._x = _x + arg._x;
		pos._y = _y + arg._y;

		return pos;
	}

	Position operator+ (int arg)  // 인자를 정수로 받았습니다.
	{
		Position pos;
		pos._x = _x + arg;
		pos._y = _y + arg;

		return pos;
	}

	bool operator== (const Position& arg)
	{
		return _x == arg._x && _y == arg._y;
	}

	/*void operator= (int arg)
	{
		_x = arg;
		_y = arg;
	}*/

	// 주로 자기자신 클래스 참조 타입의 함수로 만든다고 합니다.
	Position& operator= (int arg)
	{
		_x = arg;
		_y = arg;

		// this라는 문법을 사용하는데 this는 자기자신을 가리키고있는 포인터변수였습니다.
		// Position* this = 내자신의 주소;
		return *this;
	}

	// 복사 대입 연산자
	Position& operator= (const Position& arg)  // const를 붙이는 이유는 후위형 증감연산자 같은 케이스 때문
	{
		_x = arg._x;
		_y = arg._y;

		
		return *this;
	}

	// 증감 연산자 (전위형 , 후위형)
	Position&	 operator++() // 전위형
	{
		_x++;
		_y++;
		return *this;
	}
	Position operator++(int)  // 후위형
	{
		Position ret = *this;  // 연산 이전의 값을 ret에 저장합니다.
		_x++;
		_y++;
		return ret;
	}


public:
	int _x;
	int _y;
};

Position operator+ (int a, const Position& b)
{
	Position ret;

	ret._x = a + b._x;
	ret._y = a + b._y;

	return ret;
}

// operator= 은 멤버 함수여야 한다고 오류가 뜹니다.
//void operator= (Position& a, int b)
//{
//	a._x = b;
//	a._y = b;
//}

int main()
{
	// 합 연산자
	Position pos;
	pos._x = 0;
	pos._y = 0;

	Position pos2;
	pos2._x = 1;
	pos2._y = 1;

	Position pos3 = pos + pos2;
	// pos3 = pos.operator+(pos2); // 위와 같은 의미

	Position pos4 = pos3 + 1;

	// 멤버 연산자 함수만 있었을 때
	//pos4 = 1 + pos3; // 왼쪽의 데이터가 클래스가 아니기 때문에 멤버 연산자 함수로는 실행이 되지 않아요 

	// 전역 연산자 함수일 경우
	Position pos5 = 1 + pos3;

	// 비교연산자
	bool isSame = (pos3 == pos4);

	// 대입 연산자
	Position pos6;  
	pos6 = 5;
	// 혹시 Position pos6 = 5; 로 대입하려고 하면 이 코드는 Position pos6(5); 같이 받아 들여서 인자를 받는 생성자 함수를 찾게 됩니다.

	// 대입 연산자 함수를 자신의 참조 타입으로 만든 후에는 대입 연산후 그 값을 바로 다른 변수에 넣어 줄 수 있게 되었습니다.
	pos3 = (pos6 = 6);

	// 증감 연산자
	pos6 = ++pos3;

	// 후위형의 경우 어순상 pos5에 pos3이라는 클래스를 대입하는게 먼저이니 복사대입연산자(리턴값이 참조)가 실행되고 , 후위형 증감연산자 자체는 리턴값이 복사된 클래스입니다. 
	// (pos&) 줘~  (pos)복사값 줄게~ (pos의 복사값은 멤버 연산자 함수의 스택상에 있으므로 그냥 pos의 주소를 줄 수도 없습니다.)
	// 그래서 복사대입 연산자의 인자를 const타입으로 받으면 값을 대입한 후 수정 하지 않을 것이라는 약속?을 한것이기 때문에 대입이 가능해집니다.
	pos5 = pos3++;

	
	return 0;
}