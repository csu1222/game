#pragma once
#include "Creature.h"
enum PlayerType
{
	PT_KNIGHT = 1,
	PT_ARCHER = 2,
	PT_MAGE = 3,
};

class Player : public Creature
{
public:
	Player(int playerType);

	virtual ~Player();

	void PrintInfo();

public:
	int _playerType;
};

class Knight : public Player
{
public:
	Knight();
};

class Archer : public Player
{
public:
	Archer();
};

class Mage : public Player
{
public:
	Mage();
};

