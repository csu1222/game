#include "47_Player.h"
#include "47_Monster.h"


void Player::KillMonster()
{
	_target2->KillMe();
	//_target2->_hp = 0;
}