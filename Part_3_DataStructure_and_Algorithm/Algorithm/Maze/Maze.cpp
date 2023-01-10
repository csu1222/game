#include "pch.h"
#include <iostream>
#include "ConsoleHelper.h"

int main()
{
	uint64 lastTick = 0;
	
	while (true) 
	{
#pragma region 프레임 관리 
		uint64 currentTick = ::GetTickCount64();
		uint64 deltaTick = currentTick - lastTick;
		lastTick = currentTick;
#pragma endregion
		// 입력

		// 로직

		// 렌더링
		
		// unicode-table.com/kr/ 에서 가져왔습니다. 검색어:U+25A0
		const char* TILE = "■";

		ConsoleHelper::SetCursorPosition(0, 0);
		ConsoleHelper::ShowConsoleCursor(false);
		ConsoleHelper::SetCursorColor(ConsoleColor::RED);

		for (int32 y = 0; y < 25; y++)
		{
			for (int32 x = 0; x < 25; x++)
			{
				cout << TILE;
			}

			cout << endl;
		}
	}
}