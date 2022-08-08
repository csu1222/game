#include "Player.h"
#include <iostream>
using namespace std;

Player::Player(int playerType) : Creature(CT_PLAYER), _playerType(playerType)
{

}

Player::~Player() { }

void Player::PrintInfo()
{
	cout << "--------------------------------------" << endl;
	cout << "[플레이어 상태]  HP:" << _hp << "  ATT:" << _attack << "  DEF:" << _defence << endl;
	cout << "--------------------------------------" << endl;
}

Knight::Knight() : Player(PT_KNIGHT)
{
	_hp = 150;
	_attack = 10;
	_defence = 5;
}

Archer::Archer() : Player(PT_ARCHER)
{
	_hp = 100;
	_attack = 15;
	_defence = 3;
}

Mage::Mage() : Player(PT_MAGE)
{
	_hp = 80;
	_attack = 25;
	_defence = 0;
}