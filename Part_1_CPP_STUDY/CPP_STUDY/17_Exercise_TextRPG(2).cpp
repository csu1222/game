#include <iostream>
using namespace std;

// Text RPG 혼자서 다시 만들어보기


enum PlayerType
{
	PT_Knight = 1,
	PT_Archer,
	PT_Mage,
};

enum MonsterType
{
	MT_Slime = 1,
	MT_orc,
	MT_Skeleten,
};

int playerType;
int playerHp;
int playerAttack;
int playerDefence;

int monsterType;
int monsterHp;
int monsterAttack;
int monsterDefence;

void EnterLobby();
void CreatePlayer();
void EnterField();
void CreateMonster();
void EnterBattle();

int main()
{
	srand(time(0));

	EnterLobby();
}

void EnterLobby()
{
	while (true)
	{
		cout << "----------------------" << endl;
		cout << "로비에 입장하였습니다." << endl;
		cout << "----------------------" << endl;

		CreatePlayer();


		cout << "-------------------------" << endl;
		cout << "필드에 입장 하시겠습니까?" << endl;
		cout << "(1) 입장    (2) 그만두기" << endl;
		cout << ">";

		int input;
		cin >> input;

		if (input == 1)
		{
			EnterField();
		}
		else
			return;



	}
}

void CreatePlayer()
{
	while (true)
	{
		cout << "----------------------" << endl;
		cout << "직업을 선택해주세요!" << endl;

		cout << "(1) 기사  (2) 궁수  (3) 법사" << endl;
		cout << ">";

		int input;
		cin >> input;

		if (input == PT_Knight)
		{
			cout << "기사를 선택하셨습니다!" << endl;
			playerType = PT_Knight;
			playerHp = 150;
			playerAttack = 15;
			playerDefence = 5;
			cout << "HP : " << playerHp << "/ ATT : " << playerAttack << "/ DEF : " << playerDefence << endl;
			return;
		}
		else if (input == PT_Archer)
		{
			cout << "궁수를 선택하셨습니다!" << endl;
			playerType = PT_Archer;
			playerHp = 110;
			playerAttack = 25;
			playerDefence = 3;
			cout << "HP : " << playerHp << "/ ATT : " << playerAttack << "/ DEF : " << playerDefence << endl;
			return;
		}
		else if (input == PT_Mage)
		{
			cout << "법사를 선택하셨습니다!" << endl;
			playerType = PT_Mage;
			playerHp = 80;
			playerAttack = 35;
			playerDefence = 0;
			cout << "HP : " << playerHp << "/ ATT : " << playerAttack << "/ DEF : " << playerDefence << endl;
			return;
		}
	}
}

void EnterField()
{
	while (true)
	{
		cout << "-----------------------" << endl;
		cout << "필드에 입장하셨습니다!" << endl;
		cout << "몬스터 생성중...!" << endl;
		cout << "-----------------------" << endl;


		CreateMonster();

		if (playerHp == 0)
			return;
	}
}

void CreateMonster()
{
	monsterType= 1 + (rand() % 3);

	switch (monsterType)
	{
	case MT_Slime:
		monsterHp = 20;
		monsterAttack = 5;
		monsterDefence = 0;
		cout << "슬라임을 조우했습니다!" << endl;
		cout << "HP : " << monsterHp << "/ ATT : " << monsterAttack << "/ DEF : " << monsterDefence << endl;
		EnterBattle();
		break;
	case MT_orc:
		monsterHp = 50;
		monsterAttack = 15;
		monsterDefence = 3;
		cout << "오크를 조우했습니다!" << endl;
		cout << "HP : " << monsterHp << "/ ATT : " << monsterAttack << "/ DEF : " << monsterDefence << endl;
		EnterBattle();
		break;
	case MT_Skeleten:
		monsterHp = 80;
		monsterAttack = 20;
		monsterDefence = 5;
		cout << "해골을 조우했습니다!" << endl;
		cout << "HP : " << monsterHp << "/ ATT : " << monsterAttack << "/ DEF : " << monsterDefence << endl;
		EnterBattle();
		break;
	}
}

void EnterBattle()
{	

	cout << "(1) 전투!  (2) 도망간다" << endl;
	cout << ">";
	int input;
	cin >> input;
	if (input == 1)
	{

		while (true)
		{

			int damage = playerAttack - monsterDefence;
			if (damage < 0)
				damage = 0;

			// 선빵
			cout << "플레이어가 몬스터를 공격합니다!" << endl;

			monsterHp -= damage;
			if (monsterHp < 0)
				monsterHp = 0;

			cout << "몬스터의 HP : " << monsterHp << endl;

			if (monsterHp == 0)
			{
				cout << "몬스터 처치!" << endl;
				return;
			}

			// 반격
			damage = monsterAttack - playerDefence;
			if (damage < 0)
				damage = 0;

			cout << monsterType << "이(가) 플레이어에게 반격 했습니다!" << endl;

			playerHp -= damage;
			if (playerHp < 0)
				playerHp = 0;

			cout << "당신의 HP : " << playerHp << endl;

			if (playerHp == 0)
			{
				cout << "YOU DIED" << endl;
				return;
			}
		}
	}
	else
		return;
}