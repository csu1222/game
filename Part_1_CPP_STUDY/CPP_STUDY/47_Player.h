#pragma once

//#include "47_Monster.h"

class Player
{
public:
	void KillMonster();

public:
	int _hp;
	int _attack;

	//Monster _target;
	class Monster* _target;

	Player* _target2;
};

