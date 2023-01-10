#include <iostream>
using namespace std;

// Text RPG ȥ�ڼ� �ٽ� ������


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
		cout << "�κ� �����Ͽ����ϴ�." << endl;
		cout << "----------------------" << endl;

		CreatePlayer();


		cout << "-------------------------" << endl;
		cout << "�ʵ忡 ���� �Ͻðڽ��ϱ�?" << endl;
		cout << "(1) ����    (2) �׸��α�" << endl;
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
		cout << "������ �������ּ���!" << endl;

		cout << "(1) ���  (2) �ü�  (3) ����" << endl;
		cout << ">";

		int input;
		cin >> input;

		if (input == PT_Knight)
		{
			cout << "��縦 �����ϼ̽��ϴ�!" << endl;
			playerType = PT_Knight;
			playerHp = 150;
			playerAttack = 15;
			playerDefence = 5;
			cout << "HP : " << playerHp << "/ ATT : " << playerAttack << "/ DEF : " << playerDefence << endl;
			return;
		}
		else if (input == PT_Archer)
		{
			cout << "�ü��� �����ϼ̽��ϴ�!" << endl;
			playerType = PT_Archer;
			playerHp = 110;
			playerAttack = 25;
			playerDefence = 3;
			cout << "HP : " << playerHp << "/ ATT : " << playerAttack << "/ DEF : " << playerDefence << endl;
			return;
		}
		else if (input == PT_Mage)
		{
			cout << "���縦 �����ϼ̽��ϴ�!" << endl;
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
		cout << "�ʵ忡 �����ϼ̽��ϴ�!" << endl;
		cout << "���� ������...!" << endl;
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
		cout << "�������� �����߽��ϴ�!" << endl;
		cout << "HP : " << monsterHp << "/ ATT : " << monsterAttack << "/ DEF : " << monsterDefence << endl;
		EnterBattle();
		break;
	case MT_orc:
		monsterHp = 50;
		monsterAttack = 15;
		monsterDefence = 3;
		cout << "��ũ�� �����߽��ϴ�!" << endl;
		cout << "HP : " << monsterHp << "/ ATT : " << monsterAttack << "/ DEF : " << monsterDefence << endl;
		EnterBattle();
		break;
	case MT_Skeleten:
		monsterHp = 80;
		monsterAttack = 20;
		monsterDefence = 5;
		cout << "�ذ��� �����߽��ϴ�!" << endl;
		cout << "HP : " << monsterHp << "/ ATT : " << monsterAttack << "/ DEF : " << monsterDefence << endl;
		EnterBattle();
		break;
	}
}

void EnterBattle()
{	

	cout << "(1) ����!  (2) ��������" << endl;
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

			// ����
			cout << "�÷��̾ ���͸� �����մϴ�!" << endl;

			monsterHp -= damage;
			if (monsterHp < 0)
				monsterHp = 0;

			cout << "������ HP : " << monsterHp << endl;

			if (monsterHp == 0)
			{
				cout << "���� óġ!" << endl;
				return;
			}

			// �ݰ�
			damage = monsterAttack - playerDefence;
			if (damage < 0)
				damage = 0;

			cout << monsterType << "��(��) �÷��̾�� �ݰ� �߽��ϴ�!" << endl;

			playerHp -= damage;
			if (playerHp < 0)
				playerHp = 0;

			cout << "����� HP : " << playerHp << endl;

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