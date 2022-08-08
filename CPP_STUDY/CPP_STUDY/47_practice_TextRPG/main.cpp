#include "Game.h"
#include <iostream>
using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));

	Game game;

	game.Init();

	while (true)
	{
		game.Update();
	}

	return 0;
}