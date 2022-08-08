#pragma once
class Monster;
class Player;

class Field
{
public:
	Field();
	~Field();

	void Update(Player* player);

	void CreateMonster();

	void StartBatle(Player* player);
public:
	Monster* _monster;

};

