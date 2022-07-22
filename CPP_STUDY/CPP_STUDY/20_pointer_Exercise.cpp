#include <iostream>
using namespace std;

// ������ ���� : ������ �ǽ�


struct StatInfo		// �� 12����Ʈ ũ�Ⱑ �ɰ��Դϴ�.
{
	int hp;		// +0
	int attack;	// +4
	int defence;// +8
};
/*
�������� 
int hp;			// +0
int attack;		// +4
int defence;	// +8
�̷��� ������ ��������ϴ�.
������󿡼��� struct �� ������ ���峪 �Ȱ��ҽ��ϴ�.
*/

void EnterLobby();
StatInfo CreatePlayer();				// ouput�� StatInfo Ÿ���� ��� ���
void CreateMonster(StatInfo* info);		// �Ű������� StatInfo �����͸� �޴� ���
// bool �� ������ �÷��̾ �¸� �ߴ��� ���ߴ��� ��� �� ���̱� ����
bool StartBattle(StatInfo* player, StatInfo* monster);  

int main()
{
	EnterLobby();

	return 0;
}

void EnterLobby()
{
	cout << "�κ� �����߽��ϴ�." << endl;

	StatInfo player;
	player.hp = 0xbbbbbbbb;			// �� �κ��� ����� �Ҷ� ���� �߶�� ���ؼ� ������ ���� �־� ���� �̴ϴ�.
	player.attack = 0xbbbbbbbb;
	player.defence = 0xbbbbbbbb;

	// [�Ű�����][RET][��������(temp(100, 10, 2) player (b,b,b,...)]  [�Ű�����][RET][��������(ret(100, 10, 2))]
	// CreatePlayer �Լ��� �ƿ�ǲ���� StatInfo ����ü�� ��� �Լ� �����ϴ�. CreatePlayer�� player ������ ���� 
	// ���� ���ִ� ������ ���캸��
	// 1) ���� hp, attack, defence ������ ���ڸ��� ������ ���ִ°� Ȯ�� �߽��ϴ�.
	// 2) CreatePlayer�Լ��� �����ϱ����� ������ � �ּҸ� push ���־����ϴ�. �� ������ ���Ƿ� temp��� �ϰڽ��ϴ�.
	// 3) push�� ���־��ٴ� ���� CreatePlayer�Լ��� �Ű������� �־��ٴ� �ǹ��Դϴ�.
	// 4) CreatePlayer�Լ��� �������� ebp-8, ebp-12m ebp-16�� ���� defence��, attack��, hp���� �־��־����ϴ�.
	// 5) CreatePlayer�Լ��� ���ÿ��� ebp+8 ���� ����Ű�� �ּҷ� �� ret.hp, ret.attack, ret.defence �� ���ʷ� �־��־����ϴ�.
	// 6) �� ebp+8�� ���Ƿ� �̸� ������ temp�� �ּҿ����ϴ�. �� �� �����ϴ� ���� ret ��ɾ�� ��򰡿� �̵��˴ϴ�.
	// 7) ���� temp�� ����� ������ player����ü�� ������ ���� �־��־����ϴ�.

	// ������ڸ� 
	// 1) �Լ� ����� temp��� ������ �ּҿ� ������ �����ϰ� 
	// 2) �� ���� �ٽ� player�� �������־����ϴ�.
	// temp��� ������ �ּҸ� ���ļ� ���� ���� �޴°� ���� ��ȿ�������� ���Դϴ�.

	// ������ ���÷� Info ���� 3�� �ۿ� ������ ���� ���񽺵Ǵ� ���ӿ����� Info�� ũ�Ⱑ ��õ ����Ʈ�� �� ���� �ִٰ� �մϴ�.
	// �׷��� ����ó�� ���������� ���� �����ϰ�, �Ű������� �������� �����ϰ�, �� �Ű������� ���� �ٽ� ���� ������ �����ϰ�,
	// �ϴ� ������ ��ġ�� ���������� ���� ���ظ� ���� �� �� �ֽ��ϴ�.
	player = CreatePlayer();


	
	StatInfo monster;
	monster.hp = 0xbbbbbbbb;			// �� �κ��� ����� �Ҷ� ���� �߶�� ���ؼ� ������ ���� �־� ���� �̴ϴ�.
	monster.attack = 0xbbbbbbbb;
	monster.defence = 0xbbbbbbbb;

	// �̹����� CreateMonster�� ���캸�ڽ��ϴ�. CreateMonster�� �����͸� �̿��ؼ� ���� �߾����ϴ�.
	// 1) monster�� ������ ���� bbbb�� �Ǿ��ִ°� Ȯ���߽��ϴ�.
	// 2) CreatePlayer�� ó�� ������ temp�ּҰ� �ƴ� monster������ CreateMonster�� �Ű������� push ���־����ϴ�.
	// 3) ���� CreateMonster�Լ� ���η� ���� ���� ���� ������ �� �� �Ű������� ���� monster������ ���� ���۵Ǿ����ϴ�.
	
	CreateMonster(&monster);

	// CratePlayer ó�� ���ϰ����� ���� �����ϳ� CreateMonster�� �����͸� ���� �޾� ���� �����ϳ�
	// ����� ��������, �ӵ��鿡�� ���� ���̰� �־����ϴ�.


	// ������ 1)
	// ����ü ���� ���� �� �� �������� ��������?
	/*player = monster;
	
	player.hp = monster.hp;
	player.attack = monster.attack;
	player.defence = monster.defence;*/
	// ���� �ڵ�� �Ʒ� ������ ���� �־��ִ� �ڵ尡 ��ǻ� ���� ������ �ϰ��ֽ��ϴ�.

	bool victory = StartBattle(&player, &monster);

	if (victory)
		cout << "VICTORY!" << endl;
	else
		cout << "YOU LOSE!" << endl;
}

StatInfo CreatePlayer()
{
	StatInfo ret;

	cout << "�÷��̾� ����!" << endl;

	ret.hp = 100;
	ret.attack = 10;
	ret.defence = 2;

	return ret;
}

void CreateMonster(StatInfo* info)
{

	cout << "���� ����!" << endl;

	info->hp = 40;
	info->attack = 8;
	info->defence = 1;
}

bool StartBattle(StatInfo* player, StatInfo* monster)
{
	while (true)
	{
		int damage = player->attack - monster->defence;
		if (damage < 0)
			damage = 0; 

		monster->hp -= damage;
		if (monster->hp < 0)
			monster->hp = 0;
		cout << "monster's HP : " << monster->hp << endl;

		if (monster->hp == 0)
			return true;

		damage = monster->attack - player->defence;
		if (damage < 0)
			damage = 0;

		cout << "player's HP : " << monster->hp << endl;

		player->hp -= damage;
		if (player->hp < 0)
			player->hp = 0;

		if (player->hp == 0)
			return false;
	}
}