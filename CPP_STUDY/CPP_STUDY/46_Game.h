#pragma once

// 전방선언
class Player;
class Field;

// is - a
// has - a

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Update();

	void CreatePlayer();

private:

	// 힙영역에 생성될 플레이어 객체를 가리킬 주소 바구니
	Player* _player;
	Field* _field;
};

