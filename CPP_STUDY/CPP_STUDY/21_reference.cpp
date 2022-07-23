#include <iostream>
using namespace std;

// 오늘의 주제 : 참조 

#pragma region 포인터 복습 참조의 필요성
// 참조는 포인터와 밀접한 관련이 있습니다.
// 이전 실습을 복습하면서 참조의 필요성을 알아보겠습니다.

struct StatInfo
{
	int hp;			// +0
	int attack;		// +4
	int defence;	// +8
};

// [매개변수][RET][지역변수(info)] [매개변수(&info)][RET][지역변수]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 15;
	info->defence = 5;
}

// 값을 수정하지 않는다면 매개변수로 포인터를 주는게 아닌 값을 복사해주는것도 문제가 없습니다.
// 결과는 같지만 구현 과정에는 차이가 있습니다. 
// 값 전달 방식의 경우 매개변수로 받은 값을 임의의 지역변수에 복사하여 출력한 것
// 주소 전달 방식의 경우 매개변수로 받은 포인터가 가리키는 주소를 직접 출력한 것
// 
// StatInfo 구조체가 1000바이트짜리 대형 구조체라면?
// - (값 전달) StatInfo로 넘기면 1000바이트가 복사되는
// - (주소 전달) StatInfo*로 넘기면 8바이트(주소의 길이)
// - (참조 전달) 사실 3번째 옵션으로 이번 주제인 참조전달이 있다고 합니다.
// -  ㄴ StatInfo&로 넘기면 8바이트(주소의 길이)
// 
// 1) 값 전달 방식
void PrintInfoByCopy(StatInfo info)
{
	cout << "-------값 전달 방식 -------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.attack << endl;
	cout << "DEF : " << info.defence << endl;
	cout << "---------------------------" << endl;
}

// 2) 주소 전달 방식	
void PrintInfoByPtr(StatInfo* info)
{
	cout << "-------주소 전달 방식 -------" << endl;
	cout << "HP : " << info->hp << endl;
	cout << "ATT : " << info->attack << endl;
	cout << "DEF : " << info->defence << endl;
	cout << "---------------------------" << endl;
}
#pragma endregion

// 3) 참조 전달 방식
void PrintInfoByRef(StatInfo& info)
{
	cout << "-------참조 전달 방식 -------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.attack << endl;
	cout << "DEF : " << info.defence << endl;
	cout << "---------------------------" << endl;
}

// 값 전달, 주소 전달, 그리고 제 3의 옵션 참조 전달

int main()
{
	// 포인터 복습
	StatInfo info;
	CreateMonster(&info);
	PrintInfoByCopy(info);
	PrintInfoByPtr(&info);

	PrintInfoByRef(info);
	// 참조 설명


	// 다시 집어보자면 밑의 코드는 number라는 이름의 바구니에 1이라는 값을 넣어놓은 것
	// 이후 number에서 뭘 넣거나 꺼내려고 하면 찰떡같이 알아듣고 number의 주소(.data, .stack, .heap)로 가서 조작한다!
	int number = 1;

	// * 주소를 담는 바구니
	// type* 으로 쓰이면 type을 담는 포인터 바구니를 만든다
	// *포인터이름 으로 쓰이면 변수의 주소가 가리키는 곳으로 슝~ 가라!
	// &변수이름 은 변수의 주소를 뜻함
	int* pointer = &number;
	*pointer = 2;

	// 참조의 표현법(&)
	// 포인터와 비슷한 형태이지만 *대신 &을 사용
	// 로우레벨(어셈블리) 관점에서 실제 작동 방식은 int* 와 똑같음
	// 하지만 c++에서 사용성은 일반 변수에 가깝다!
	int& reference = number; 

	// C++ 관점에서는 number라는 바구니에 또 다른 이름을 부여한것
	// number라는 바구니에 reference라는 다른 이름을 지어줄게~
	// 앞으로 reference 바구니에 뭘 꺼내거나 넣으면
	// 실제 number 바구니(진퉁에다가) 그 값을 꺼내거나 넣으면 됨!

	reference = 3;

	// 그런데 귀찮게 또 다른 이름을 짓는이유?
	// 그냥 number = 3; 이라고 해도 똑같은데...
	// 그 이유는 함수에서 매개변수로 전달할 때 더 편하게 하기 위해


	return 0;
}