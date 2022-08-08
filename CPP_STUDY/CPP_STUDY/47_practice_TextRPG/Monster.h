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
	virtual ~Monster();

	void PrintInfo();
public:
	int _monsterType;
};

class Slime : public Monster
{
public:
	Slime();
};

class Orc : public Monster
{
public:
	Orc();
};

class Skeleton : public Monster
{
public:
	Skeleton();
};

