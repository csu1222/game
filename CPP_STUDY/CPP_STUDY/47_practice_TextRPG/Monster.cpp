#include <iostream>
#include "Monster.h"
using namespace std;

Monster::Monster(int monsterType) : Creature(CT_MONSTER), _monsterType(monsterType)
{

}

Monster::~Monster() { }

void Monster::PrintInfo()
{
	cout << "-------------------------" << endl;
	cout << "[몬스터 정보] HP:" << _hp << " ATT:" << _attack << " DEF:" << _defence << endl;
	cout << "-------------------------" << endl;
}