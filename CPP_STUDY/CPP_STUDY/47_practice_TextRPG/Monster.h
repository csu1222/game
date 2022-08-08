#pragma once
#include "Creature.h"

enum MonsterType
{
	MT_SLIME = 1,
	MT_ORC = 2,
	MT_SKELETON = 3,
};


class Monster : public Creature
{
public:
	Monster(int monsterType);

	~Monster();

	void PrintInfo();

public:
	int _monsterType;
};

class Slime : public Monster
{
public:
	Slime() : Monster(MT_SLIME)
	{
		_hp = 30;
		_attack = 5;
		_defence = 0;
	}
public:
	
};

class Orc : public Monster
{
public:
	Orc() : Monster(MT_ORC)
	{
		_hp = 40;
		_attack = 8;
		_defence = 2;
	}
public:
	
};

class Skeleton : public Monster
{
public:
	Skeleton() : Monster(MT_SKELETON)
	{
		_hp = 50;
		_attack = 10;
		_defence = 5;
	}
public:
	
};

