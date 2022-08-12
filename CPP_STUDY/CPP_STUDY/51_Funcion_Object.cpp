#include <iostream>
using namespace std;

// 오늘의 주제 : 함수 객체


/*
함수 객체 (Functor) : 함수 처럼 동작하는 객체

함수 객체의 필요성
 - 우선, 함수포인터의 단점 :
		1)  정의한 시그니쳐와 같은 형태의 함수만 사용할 수 있었습니다.
		2) 상태를 가질 수 없었습니다. ( 상태 = 함수에 저장되는 값 , 함수만 생각하면 무슨 말인지 어렵지만 클래스를 예를들면
			클래스는 상태(멤버 변수)와 동작(멤버 함수)을 함께 가지고 있을수 있습니다.)

함수 포인터의 단점을 커버해주는 것이 함수 객체 (그래도 함수 객체에 대해 이해하려면 함수포인터에 대해 알고 있어야 겠습니다.)

함수 객체란? 
- [함수처럼 동작]하는 객체
예를들어 HelloWorld라는 객체를 함수처럼 HelloWorld(); 이렇게 사용하려면?
- () 연산자 오버로딩 이 필요하겠습니다.

[연산자 오버로딩] 복습 : 멤버 함수 선언처럼 반환타입 인자 까지 비슷하지만 함수이름이 operator() 이런 형식 이었습니다.

- () 연산자 오버로딩에서 기능을 구현하고 함수처럼 쓰입니다.
- 함수 포인터와 다른점은 기능도 있고 상태도 들고 있을수 있다는 것



	MMO에서 함수 객체를 사용하는 예시
	클라 <-> 서버 
	서버 : 클라가 보내준 네트워크 패킷을 받아서 처리
	ex) 클라: 나 (5, 0) 좌표로 이동시켜줘!
*/

// 함수 객체 
class Functor
{
public:
	// () 연산자를 오버로딩하였습니다.
	void operator() ()
	{
		cout << "Functor Test" << endl;
		cout << _value << endl;
	}

	// 상상력을 동원해서 여러 형태의 연산자 오버로딩이 가능해 집니다.
	bool operator() (int a)			// 반환 타입과 인자의 의미는 없지만 여러 형태가 가능하다는 의미
	{
		cout << "Functor Test" << endl;
		_value += a;
		cout << _value << endl;

		return true;
	}

private:
	int _value = 0;
};

// MMO에서 함수객체를 사용하는 예시
class MoveTask
{
public:
	void operator() ()
	{
		// TODO
		cout << "해당 좌표로 플레이어 이동" << endl;
	}
public:
	int _playerId;
	int _poxX;
	int _posY;
};



int main()
{
	// ()연산자 오버로딩으로 함수처럼 실행하였습니다.
	Functor functor;

	functor();		// 형태는 함수와 유사하지만 연산자입니다.
	bool ret = functor(3);


	// MMO에서 함수 객체를 사용하는 예시
	// 클라 : 나(playerId가 100번)를 (0, 5)로 이동시켜줘! 
	// 라는 요청을 객체로 저장 해두고 순서대로 실행할수 있습니다.
	MoveTask task;
	task._playerId = 100;	//  요청을 저장하는 부분
	task._posY = 5;
	task._poxX = 0;

	task();					// 저장해 뒀던 요청을 실행하는 부분

	return 0;
}