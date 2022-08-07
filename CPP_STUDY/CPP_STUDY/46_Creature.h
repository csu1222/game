#pragma once

enum CreatureType
{
	CT_PLAYER = 1,
	CT_MONSTER = 2,
};

class Creature
{
public:
	Creature(int creatureType) : _creatureType(creatureType), _hp(0), _attack(0), _defence(0)
	{

	}

	virtual ~Creature()
	{

	}

	virtual void PrintInfo() = 0;

	void OnAttacked(Creature* attacker);

	bool IsDead() { return _hp <= 0; } // �Լ��� �۱⋚���� ������Ͽ� ����


protected:
	int _creatureType;
	int _hp;
	int _attack;
	int _defence;
};

