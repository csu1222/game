#pragma once

// ���漱��
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

	// �������� ������ �÷��̾� ��ü�� ����ų �ּ� �ٱ���
	Player* _player;
	Field* _field;
};

