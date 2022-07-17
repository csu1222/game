#include <iostream>
using namespace std;

// 오늘의 주제 : 분기문
// 데이터를 메모리에 할당하고 가공하는 방법에 대해 알아봄
// 가공한 데이터를 이용해서 무엇인가를 하고 싶다면?

int main()
{
#pragma region 분기문 if문
	int hp = 100; // 몬스터의 체력
	int damage = 100; // 플레이어 데미지

	hp -= damage;  // 피격 판정
	bool isDead = (hp <= 0);  // 처치 판정

	// 몬스터가 죽었으면 경험치 추가
	// 어셈블리에서는 cmp, jmp

	// 대표적으로 if문
	// if (조건)   // 조건으로 무조건 불리언 값이 와야하는 것은 아닙니다. 
	//	 명령문
	
	if (isDead)
		cout << "몬스터를 처치 했습니다." << endl;	

	if (!isDead)  // 혹은 (isDead == false)
		cout << "몬스터가 반격 했습니다." << endl;
	// if문에서 위에 조건이 참으로 판별되어 실행되면 사실상 밑의 if문은 보지 않아도 될 것입니다.
	// 하지만 지금처럼 if문을 두번 연달아 사용해 구현한 경우 무조건 둘다 판별을 실행해 연산능력을 차지 하게 됩니다.

	if (isDead)
		cout << "몬스터를 처치했습니다." << endl;
	else
		cout << "몬스터가 반격했습니다." << endl;
	// if else 문으로 조건을 판별하면 첫 조건에 부함되면 자동으로 밑의 명령문은 패스 하게 됩니다.

	// if 문에서 두 줄이상의 명령을 하고 싶다면 { }를 사용해야합니다.
	if (isDead)
	{
		cout << "몬스터를 처치했습니다." << endl;
		cout << "몬스터를 처치했습니다." << endl;
	}

	// if 문에서 조건이 여러개일경우
	 if(true) 
	 {
	 }
	 else
	 {
		 if (true)
		 {
		 }
		 else
		 {
		 }
	 }
	 // 이렇게 if else 문을 중첩 시킬 수도 있지만 코드가 길어지면 가독성이 매우 떨어 지게 됩니다.

	 //그래서 if - else if - else 문을 사용합니다.

	 if (isDead) 
	 {
		 cout << "몬스터를 처치했습니다." << endl;
	 }
	 else if (0 < hp <= 20)
	 {
		 cout << "몬스터가 도망가고 있습니다." << endl;
	 }
	 else 
	 {
		 cout << "몬스터가 반격했습니다." << endl;
	 }
#pragma endregion

#pragma region switch case break 문
	 const int ROCK = 0;
	 const int PAPER = 1;
	 const int SCISSORS = 2;

	 int input = ROCK;

	 if (input == ROCK)
		 cout << "바위를 냈습니다." << endl;
	 else if (input == PAPER)
		 cout << "보를 냈습니다." << endl;
	 else if (input == SCISSORS)
		 cout << "가위를 냈습니다." << endl;
	 else
		 cout << "뭘 낸겁니까?" << endl;

	 // if else 문은 효율상 문제는 없지만 코드가 길어지면 반복이 계속 되고 오타가 생길수도 있습니다.


	 // switch - case  - break 문
	 // 가독성을 좋아지지만 성능상으로는 if else 문과 비슷합니다.
	 // switch의 인자로 정수 계열만 넣을 수 있습니다. (float, string 계열은 안됩니다.)

	 switch (input)
	 {
	 case ROCK:
		 cout << "바위를 냈습니다." << endl;
			break;
	 case PAPER:
		 cout << "보를 냈습니다." << endl;
			break;
	 case SCISSORS:
		 cout << "가위를 냈습니다." << endl;
			break;
	 default:
		 cout << "뭘 낸겁니까" << endl;
		 break;
	 }
#pragma endregion
}