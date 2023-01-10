#include <iostream>
using namespace std;

// 오늘의 주제 : 은닉성(Data hiding)

// 객체지향 (OOP Object Oriented Programming)
// - 상속성   
// - 은닉성  *
// - 다형성

/*
 은닉성(Data Hiding) = 캡슐화(Encapsulation)
 몰라도 되는것은 깔끔하게 숨기겠다!
 숨기는 이유? (협업상황에서의 안전 예방)
 1)	정말 위험하고 건드리면 안되는 경우
 2) 다른 경로로 접근하길 원하는 경우

 세가지 키워드 public(공개적), protected(보호받는), private(개인적인)
 이 키워드들을 '(멤버) 접근 지정자' 라고 합니다.
 public : 누구한테나 공개 하겠다.  실컷 사용하세요~~
 protected : 나의 자손들한테만 허락
 private : 나만 사용할꺼! << class Car 내부에서만 사용가능


 상속할때 붙는 public은 '상속 접근지정자' 입니다.
 상속 접근 지정자 : 다음 세대에게 부모님의 유산을 어떻게 물려줄지?
 부모님한테 물려받은 유산을 꼭 자식들에게도 똑같이 물려줘야 하진 않음
 public : (대부분의 상황에서 쓰일것) 부모님의 유산을 설계 그대로 물려줌 (public->public, protected->protected)
 protected : 내 자손들에게만 물려줄 것 (public->protected, protected->protected)
 private : 나까지만 잘 쓰고 자손들에게는 안 물려줄거야 (public->private, protected->private)
 (주의) 상속 접근 지정자를 까먹고 작성 안하면 private로 설정됩니다.
 */


// 1) 위험하고 건드리면 안되는 경우
#pragma region 예시 자동차
// 예시로 자동차
// - 핸들
// - 패달
// - 엔진
// - 문
// - 각종 전기선

// 일반 구매자 입장에서 사용하는 것?
// - 핸들/ 패달/ 문
// 몰라도 되는 부분(오히려 건드리면 큰일남)
// - 엔진/ 각종 전기선
#pragma endregion

class Car
{
public:
	void MoveHandle() {	}
	void PushPedal() { }
	void OpenDoor() { }

	void Turnkey()
	{
		// ... 
		RunEngine();
	}

//private:  // 
protected:  // 자손들 내부에서 까지 사용가능
	void DisassembleCar() { } // 차를 분해한다
	void RunEngine() { } // 엔진을 구동시킨다
	void ConnectCircuit() { } // 전기선 연결

public:
	// 핸들
	// 패달
	// 엔진
	// 문
	// 각종 전기선
};

class SuperCar : public Car // 상속 접근 지정자
{
public:
	void PushRemoteController()
	{
		RunEngine();
	}
};


// 2) 다른경로로 접근하길 원하는 경우
// '캡슐화'
// - 연관된 데이터와 함수를 논리적으로 묶어놓은 것

class Berserker
{
public:
	// 사양 : 체력이 50 이하로 떨어지면 버서커 모드 발동
	int GetHp() { return _hp; }
	void SetHp(int hp)
	{
		_hp = hp;
		if (hp < 50)
			SetBerserkerMode();
	}

private:
	void SetBerserkerMode()
	{
		cout << "매우 강해짐" << endl;
	}

private:
	int _hp = 100;
};


int main()
{
	Car car;
	
	Berserker b;

	b.SetHp(100);
	


	return 0;
}
