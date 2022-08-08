#include "Creature.h"

Creature::Creature(int cretureType) : _cretureType(cretureType), _hp(0), _attack(0), _defence(0)
{
}

Creature::~Creature() { }

void Creature::OnAttacked(Creature* attacker)
{
	int damage = attacker->_attack - _defence;

	if (damage < 0)
		damage = 0;

	_hp -= damage;

	if (_hp < 0)
		_hp = 0;
}