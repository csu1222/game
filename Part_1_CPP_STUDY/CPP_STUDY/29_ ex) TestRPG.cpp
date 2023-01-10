#include <iostream>
using namespace std;

// 설계
// 로비
// - 플레이어 생성
// - 게임입장
// -- 몬스터생성
// -- 전투 

enum PlayerType
{
	PT_Knight = 1,
	PT_Archer,
	PT_Mage,
};

enum MonserType
{
	MT_Slime = 1,
	MT_Orc,
	MT_Skeleton,
};

struct StatInfo
{
	int hp;
	int attack;
	int defence;
};
void EnterLobby();
void PrintMsg(const char* msg);
void CreatePlayer(StatInfo& playerInfo);
void EnterGame(StatInfo& playerInfo);
void PrintStat(const char* type,StatInfo& playerInfo);
void CreateMosters(StatInfo* monsterInfo, int count);
bool EnterBattle(StatInfo& playerInfo, StatInfo& monsterInfo);

int main()
{
	srand((unsigned)time(nullptr));

	EnterLobby();
	return 0;
}

void EnterLobby()
{
	while (true)
	{
		PrintMsg("로비에 입장하였습니다.");
		
		StatInfo playerInfo;
		CreatePlayer(playerInfo);
		
		PrintMsg("캐릭터 생성 완료");
		PrintStat("Player", playerInfo);

		EnterGame(playerInfo);
	}
}

void PrintMsg(const char* msg)
{
	cout << "----------------------------------" << endl;
	cout << msg << endl;
	cout << "----------------------------------" << endl;

}
void CreatePlayer(StatInfo& playerInfo)
{
	bool choice = false;
	while (choice == false)
	{
		PrintMsg("직업을 선택 해주세요");
		PrintMsg("[1] 기사  [2] 궁수  [3] 법사");
		cout << ">";

		int input;
		cin >> input;

		switch (input)
		{
		case PT_Knight:
			playerInfo.hp = 100;
			playerInfo.attack = 10;
			playerInfo.defence = 10;
			choice = true;
			break;
		case PT_Archer:
			playerInfo.hp = 80;
			playerInfo.attack = 15;
			playerInfo.defence = 5;
			choice = true;
			break;
		case PT_Mage:
			playerInfo.hp = 50;
			playerInfo.attack = 25;
			playerInfo.defence = 3;
			choice = true;
			break;
		}

	}
}

void PrintStat(const char* type , StatInfo& playerInfo)
{
	cout << "----------------------------------" << endl;
	cout << type << " : HP = " << playerInfo.hp << " ATT = " << playerInfo.attack << " DEF = " << playerInfo.defence << endl;
	cout << "----------------------------------" << endl;
}

void EnterGame(StatInfo& playerInfo)
{
	const int monster_count = 2;
	PrintMsg("게임에 입장하였습니다.");

	while (true)
	{	
		StatInfo monsterInfo[monster_count];
		CreateMosters(monsterInfo, monster_count);
		for (int i = 0; i < monster_count; i++)
			PrintStat("MONSTER", monsterInfo[i]);

		PrintMsg("[1]번째 몬스터와 전투  [2]번째 몬스터와 전투  [3] 도망간다");
		cout << ">";

		int input;
		cin >> input;
		if (input == 1 || input == 2)
		{
			int index = (input - 1);
			bool victory = EnterBattle(playerInfo, monsterInfo[index]);

			PrintStat("Player", playerInfo);
			if (victory == false)
			{
				PrintMsg("GAME OVER");
				return;
			}
		}
	}
}

void CreateMosters(StatInfo* monsterInfo, int count)
{
	PrintMsg("몬스터들을 생성합니다.");
	for (int i = 0; i < count; i++)
	{
		int randValue = 1 + (rand() % 3);
		switch (randValue)
		{
		case MT_Slime:
			monsterInfo[i].hp = 30;
			monsterInfo[i].attack = 10;
			monsterInfo[i].defence = 3;
			break;
		case MT_Orc:
			monsterInfo[i].hp = 40;
			monsterInfo[i].attack = 13;
			monsterInfo[i].defence = 4;
			break;
		case MT_Skeleton:
			monsterInfo[i].hp = 50;
			monsterInfo[i].attack = 16;
			monsterInfo[i].defence = 5;
			break;
		}
	}
	
}

bool EnterBattle(StatInfo& playerInfo, StatInfo& monsterInfo)
{
	while (true)
	{
		int damage = playerInfo.attack - monsterInfo.defence;
		if (damage < 0)
			damage = 0;

		monsterInfo.hp -= damage;
		
		if (monsterInfo.hp < 0)
			monsterInfo.hp = 0;

		PrintStat("Monster", monsterInfo);

		if (monsterInfo.hp == 0)
		{
			PrintMsg("몬스터를 처치하였습니다!");
			return true;
		}

		damage = monsterInfo.attack - playerInfo.defence;
		if (damage < 0)
			damage = 0;

		playerInfo.hp -= damage;
		if (playerInfo.hp < 0)
			playerInfo.hp = 0;
		
		if (playerInfo.hp == 0)
		{
			PrintMsg("GAME OVER");
			return false;
		}
	}

}