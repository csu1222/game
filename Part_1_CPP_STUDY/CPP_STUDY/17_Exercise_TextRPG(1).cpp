#include <iostream>
using namespace std;

// 오늘의 주제 : TEXT RPG
// -지금단계에서는 지역변수를 함수의 인자로 사용하는 방법을 안배웠으니
// -전역변수를 사용해서 구현해보겠습니다.


enum PlayerType
{
	PT_Knight = 1,
	PT_Archer = 2,
	PT_Mage = 3,
};

enum MonsterYtpe
{
	MT_Slime = 1,
	MT_Orc = 2,
	MT_Skeleton = 3,
};


struct ObjectInfo  // 이 데이터 형은 안에 int 변수 4개가 있으므로 최소 int4개 만큼의 크기를 가질 것 입니다.
{					// 하지만 항상 구성된 변수의 크기 합만큼의 크기를 가지지는 않습니다.
	int type;		// 컴퓨터가 계산하기 쉽게끔 조금의 여유를 두고 크기를 잡습니다.
	int hp;
	int attack;
	int defence;
};

ObjectInfo playerInfo;

ObjectInfo monsterInfo;

// 흩어져 있던 변수들을 하나로 모으는 문법입니다.
/*
int playerType;
int hp;
int attack;
int defence;

int monsterType;
int monsterHp;
int monsterAttack;
int monsterDefence;
*/

void EnterLobby();
void SelectPlayer();
void EnterField();
void CreateRandomMonster();
void EnterBattle();

int main()
{

	srand(time(0));  // 랜덤시드 생성

	EnterLobby();
	return 0;
}

void EnterLobby()
{
	while (true) 
	{
		cout << "--------------------" << endl;
		cout << "로비에 입장했습니다!" << endl;
		cout << "--------------------" << endl;
		
		// 플레이어 직업 선택
		SelectPlayer();

		cout << "-----------------------------" << endl;
		cout << "(1) 필드 입장  (2) 게임 종료!" << endl;
		cout << "-----------------------------" << endl;
		cout << "> ";
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

void SelectPlayer()
{
	while (true)
	{
		cout << "--------------------" << endl;
		cout << "직업을 골라주세요!" << endl;
		cout << "(1) 기사  (2) 궁수  (3) 법사" << endl;
		cout << ">";

		cin >> playerInfo.type;

		if (playerInfo.type == PT_Knight)
		{
			cout << "기사 생성중...!" << endl;
			playerInfo.hp = 150;
			playerInfo.attack = 10;
			playerInfo.defence = 5;
			return;
		}
		else if (playerInfo.type == PT_Archer)
		{
			cout << "궁수 생성중...!" << endl;
			playerInfo.hp = 100;
			playerInfo.attack = 15;
			playerInfo.defence = 3;
			return;
		}
		else if (playerInfo.type == PT_Mage)
		{
			cout << "법사 생성중...!" << endl;
			playerInfo.hp = 80;
			playerInfo.attack = 25;
			playerInfo.defence = 0;
			return;
		}
		else 
			continue;
	}
}

void EnterField()
{
	while (true)
	{
		cout << "--------------------" << endl;
		cout << "로비에 입장했습니다!" << endl;
		cout << "--------------------" << endl;

		cout << "[PLAYER] HP : " << playerInfo.hp << " / ATT : " << playerInfo.attack << " / DEF : " << playerInfo.defence << endl;
		

		CreateRandomMonster();

		cout << "--------------------" << endl;
		cout << "(1) 전투   (2) 도주" << endl;
		cout << ">";

		int input;
		cin >> input;

		if (input == 1)
		{
			EnterBattle();
			if (playerInfo.hp == 0)
			return;
		}
		else 
			return;
	}
}

void CreateRandomMonster()
{	
	//1~3
	monsterInfo.type = 1 + (rand() % 3);

	switch (monsterInfo.type)
	{
	case MT_Slime:
		cout << "슬라임 생성중...!(HP: 15 / ATT: 5 / DEF:0" << endl;
		monsterInfo.hp = 15;
		monsterInfo.attack = 5;
		monsterInfo.defence = 0;
		break;
	case MT_Orc:
		cout << "오크 생성중...!(HP: 40 / ATT: 10 / DEF:3" << endl;
		monsterInfo.hp = 40;
		monsterInfo.attack = 10;
		monsterInfo.defence = 3;
		break;
	case MT_Skeleton:
		cout << "스켈레톤 생성중...!(HP: 80 / ATT: 15 / DEF:5" << endl;
		monsterInfo.hp = 80;
		monsterInfo.attack = 15;
		monsterInfo.defence = 5;
		break;
	}
}

void EnterBattle()
{
	while (true)
	{
		int damage = playerInfo.attack - monsterInfo.defence;
		if (damage < 0)
			damage = 0;

		// 선빵
		monsterInfo.hp -= damage;
		if (monsterInfo.hp < 0)
			monsterInfo.hp = 0;

		cout << "몬스터를 공격했습니다!" << endl;
		cout << "몬스터의 남은 체력 : " << monsterInfo.hp << endl;
		if (monsterInfo.hp == 0)
		{
			cout << "몬스터를 처치했습니다!" << endl;
			return;
		}

		damage = monsterInfo.attack - playerInfo.defence;
		if (damage < 0)
			damage = 0;

		// 반격
		playerInfo.hp -= damage;
		if (playerInfo.hp < 0)
			playerInfo.hp = 0;
		cout << "공격당했습니다!" << endl;
		cout << "플레이어의 남은 체력 : " << playerInfo.hp << endl;
		if (playerInfo.hp == 0)
		{
			cout << "당신은 사망했습니다.... GAME OVER" << endl;
			return;
		}
	}
}