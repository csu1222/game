#include <iostream>
using namespace std;
#include "Knight.h"
#include "Archer.h"
#include "Mage.h"
#include "Pet.h"

// [Bug Report #9]
// 궁수가 죽으면 펫도 같이 죽는 사양이 추가되었습니다.
// 그런데 왜 프로그램도 같이 죽는걸까요?
// 이유를 찾아서 고쳐주세요!

/*
AddHp 함수에서도 궁수가 죽으면 _pet을 delete했는데 궁수 소멸자에서도 다시 delete를 해서 
더블 delete 에러가 났습니다. 

해결 AddHp에서 delete _pet 을 한후 _pet = nullptr 로 초기화해주어서 궁수 소멸자때 조건문으로 
이 중 delete를 하지 않도록 하였습니다.
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
	knight->_attack = 100;
	
	int damage = knight->GetAttackDamage();
	archer->AddHp(-damage);

	delete archer;
	delete knight;
}
