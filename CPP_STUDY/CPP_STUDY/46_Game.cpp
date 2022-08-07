#include "46_Game.h"
#include "46_Player.h"
#include "46_Field.h"
#include <iostream>
using namespace std;

// ���� �س��� �����ڿ� �Ҹ��ڸ� �����մϴ�.

Game::Game() : _player(nullptr), _field(nullptr)
{

}

Game::~Game()
{
	if (_player != nullptr)
		delete _player;
	if (_field != nullptr)
		delete _field;
}


void Game::Init()
{
	_field = new Field();
}

void Game::Update()
{
	if (_player == nullptr)
		CreatePlayer();
	
	if (_player->IsDead())
	{
		delete _player;
		_player = nullptr;
		CreatePlayer();
	}

	_field->Update(_player);
}

void Game::CreatePlayer()
{
	while (_player == nullptr)
	{
		cout << "--------------------------" << endl;
		cout << "ĳ���͸� �����մϴ�." << endl;
		cout << "[1] ���  [2] �ü�  [3] ���� " << endl;
		cout << "--------------------------" << endl;

		cout << "> ";

		int input = 0;
		cin >> input;

		if (input == PT_KNIGHT)
		{
			_player = new Knight();
		}
		else if (input == PT_ARCHER)
		{
			_player = new Archer();
		}
		else if (input == PT_MAGE)
			_player = new Mage();

	}
}