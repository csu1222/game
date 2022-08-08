#pragma once

enum CreatureType
{
	CT_PLAYER = 1,
	CT_MONSTER = 2,
};

class Creature
{
public:
	Creature(int cretureType);
	virtual ~Creature();

	virtual void PrintInfo() = 0;

	void OnAttacked(Creature* attacker);

	bool IsDead() { return _hp <= 0; }

public:
	int _cretureType;
	int _hp;
	int _attack;
	int _defence;
};

