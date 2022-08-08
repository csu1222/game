#pragma once

class Player;
class Field;


class Game
{
public:
	Game();

	~Game();

	void Init();

	void Update();
	void CreatePlayer();
public:
	Player* _player;
	Field* _field;
};

