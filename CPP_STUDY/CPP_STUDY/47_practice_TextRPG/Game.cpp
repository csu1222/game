#include "Game.h"
#include "Player.h"
#include "Field.h"
#include <iostream>
using namespace std;

Game::Game() : _player(nullptr), _field(nullptr)
{

}

Game::~Game()
{
	if (_field != nullptr)
		delete _field;
	if (_player != nullptr)
		delete _player;
}

void Game::Init()
{
	_field = new Field;
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
		cout << "--------------------------------" << endl;
		cout << "직업을 선택해주세요!" << endl;
		cout << "[1] 기사  [2] 궁수  [3] 법사" << endl;
		cout << "--------------------------------" << endl;
		cout << " > ";
		int input = 0;

		cin >> input;

		switch (input)
		{
		case PT_KNIGHT:
			_player = new Knight();
			break;
		case PT_ARCHER:
			_player = new Archer();
			break;
		case PT_MAGE:
			_player = new Mage();
			break;
		}

	}
}