#include <iostream>
#include "Field.h"
#include "Game.h"
#include "Monster.h"
#include "Player.h"

Field::Field() : _monster(nullptr)
{
	
}

Field::~Field()
{
	if (_monster != nullptr)
		delete _monster;
}

void Field::Update(Player* player)
{
	if (_monster == nullptr)
		CreateMonster();

	StartBattle(player);
}

void Field::CreateMonster()
{
	int randValue = 1 + (rand() % 3);

	if (randValue == MT_SLIME)
	{
		_monster = new Slime();
	}
	else if (randValue == MT_ORC)
	{
		_monster = new Orc();
	}
	else if (randValue == MT_SKELETON)
	{
		_monster = new Skeleton();
	}
}

void Field::StartBattle(Player* player)
{
	while (true)
	{
		player->PrintInfo();
		_monster->PrintInfo();

		_monster->OnAttacked(player);

		if (_monster->IsDead())
		{
			_monster->PrintInfo();
			delete _monster;
			_monster = nullptr;
			break;
		}

		player->OnAttacked(_monster);

		if (player->IsDead())
		{
			player->PrintInfo();
			break;
		}

	}
}