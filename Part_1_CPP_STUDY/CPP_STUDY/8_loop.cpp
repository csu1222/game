#include <iostream>
using namespace std;

// 오늘의 주제 : 반복문
// 데이터 가공의 큰 두줄기 분기문, 반복문



int main()
{
	// while -동안에
	// if-else 굉장히 유용하다
	// 하지만 한 번만 실행하는게 아니라, 특정 조건까지 반복해야 하는 경우
	// ex) 게임을 끌 때까지 계속 게임을 실행해라
	// ex) 목적지에 도달 할 때까지 계속 이동하라
	cout << "while문" << endl;

	int count = 0;

	while (count < 5)
	{
		cout << "Hello World" << endl;
		count++;
	}

	// do-while 문
	// while 문과의 차이는 do 부분의 코드를 한번은 실행하고 난 후 while 부분에서 조건을 체크하므로
	// 조건에 부합되지 않더라도 한번은 실행됩니다.

	cout << "do-while문" << endl;

	count = 0;
	do
	{
		cout << "Hello World" << endl;
		count++;
	} while (false);


	// for문  
	// while 문에서 count의 역할을 하는 변수가 반복문 밖에서 초기화되고 
	// 카운트를 증가시키는 코드도 정해진 위치가 없기 때문에 
	// 각각 코딩 스타일에 따라 해석하기 했갈릴 수 있습니다.
	// for문 에서는 초기식, 조건식, 제어식을 모아 놔 가독성이 좋습니다.

	cout << "for문" << endl;

	for (int i = 0; i < 5; i++)
	{
		cout << "Hello World" << endl;
	}


	// break; continue;
	// 루프 도중 어떤 조건이 되면 반복문을 빠져나가거나
	// 루프 도중 어떤 조건이 되면 밑의 코드를 무시하고 다음 루프로 진행 하고 싶을때


	cout << "break 문" << endl;

	int round = 1;
	int hp = 100;
	int damage = 10;

	// 무한 루프 : 전투 시작
	while (true)
	{
		hp -= damage;
		if (hp < 0)
			hp = 0; // 음수 체력을 보정

		// 시스템 메세지
		cout << "Round " << round << " 몬스터 체력 " << hp << endl;

		if (hp == 0)
		{	
			cout << "몬스터 처치!" << endl;
			break;
		}

		if (round == 5)
		{
			cout << "제한 라운드 종료" << endl;
			break;
		}
		round++;
	}
	
	// 1~ 10 사이의 홀수만 출력하기
	// continue활용
	
	for (int num = 1; num <= 10; num++)
	{
		bool isEven = ((num % 2) == 0);

		if (isEven)
			continue;	


		cout << num << endl;
	}

}