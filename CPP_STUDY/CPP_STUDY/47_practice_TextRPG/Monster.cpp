#include "Monster.h"
#include <iostream>
using namespace std;


Monster::Monster(int monsterType) : Creature(CT_MONSTER), _monsterType(monsterType)
{

}

Monster::~Monster()
{

}

void Monster::PrintInfo()
{
	cout << "--------------------------------------" << endl;
	cout << "[몬스터 상태]  HP:" << _hp << "  ATT:" << _attack << "  DEF:" << _defence << endl;
	cout << "--------------------------------------" << endl;
}

Slime::Slime() : Monster(MT_SLIME)
{
	_hp = 30;
	_attack = 5;
	_defence = 0;
}

Orc::Orc() : Monster(MT_ORC)
{
	_hp = 40;
	_attack = 6;
	_defence = 1;
}

Skeleton::Skeleton() : Monster(MT_SKELETON)
{
	_hp = 50;
	_attack = 7;
	_defence = 2;
}