#include <iostream>
using namespace std;
#include "Knight.h"
#include "Archer.h"
#include "Mage.h"
#include "Pet.h"
#include "Arrow.h"

// [Bug Report #10]
// 궁수라며면 자고로 화살을 쏴야겠죠.
// Arrow는 자신을 목표 대상물을 target으로 관리하고
// Arrow를 만들 때 설정한 데미지 만큼 target의 체력을 깎습니다.
// 테스트를 위해 화살 10개를 생성해서 기사를 피격했는데...
// C++이 늘 그렇지만 되는 일이 없고 또 크래시가 납니다.
// 뭐가 문제일까요?

/*
Arrow[6]->AttackTarget 이후 knight의 체력이 0이 되어서 delete되고 값이 nullptr이 되는데 
Arrow[] 배열의 7번째 포인터부터 가리키는 타겟이 비트열이 다 밀려서 크래쉬가나는것 같습니다.

저는 arrows[i]->AttackTarget(); 를 if (knight != nullptr) 안으로 옮겨서 해결했습니다.
*/

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	Archer* archer = new Archer(new Pet());
	archer->_hp = 100;
	archer->_maxHp = 100;
	archer->_attack = 20;

	Knight* knight = new Knight();	
	knight->_hp = 150;
	knight->_maxHp = 150;
	knight->_attack = 10;
	
	Arrow* arrows[10] = {};
	for (int i = 0; i < 10; i++)
	{
		// 기사를 타겟으로, 궁수의 공격력을 지닌 상태
		Arrow* arrow = new Arrow(knight, archer->_attack);
		arrows[i] = arrow;
	}

	for (int i = 0; i < 10; i++)
	{
			

		// 기사가 죽었으면 소멸시켜준다
		if (knight != nullptr)
		{
			arrows[i]->AttackTarget();
			if (knight->IsDead())
			{
				delete knight;
				knight = nullptr;
			}
		}	

		delete arrows[i];
		arrows[i] = nullptr;
	}
}
