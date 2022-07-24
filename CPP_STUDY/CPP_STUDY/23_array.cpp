#include <iostream>
using namespace std;

// ������ ���� : �迭

struct StatInfo
{
	int hp = 0xAAAAAAAA;
	int attack = 0xBBBBBBBB;
	int defence = 0xDDDDDDDD;
};


int main()
{	
	// TYPE �̸�[����];

	// �迭�� ũ��� ������� �� (Visual Studio ����)
	const int monstersCount = 10;
	StatInfo monsters[monstersCount];

	// ���²� �������� [�̸�]�� �ٱ����� �̸��̾���
	
	// �׷��� �迭�� [�̸�] ���� �ٸ��� �����Ѵ�
	StatInfo players[10];
	// players = monsters;    ���������� ���� ���� �� �־��µ� ������ ���ϴ�.

	// �׷� �迭�� �̸��� ����?
	// �迭�� �̸��� �� �迭�� ���� �ּ�
	// ��Ȯ���� ���� ��ġ�� ����Ű�� TYPE* ������

	auto WhoAmI = monsters;  // auto �� C++ 11 ���� ���� �������� ����� Ÿ���� �����Ͽ� Ÿ���� �ڵ����� �ٲ��ִ� ġƮŰ ���� Ÿ��
	// auto�� monsters��� �迭�� Ÿ���� �˾ƺ� ��� StatInfo* ��, StatInfoŸ���� ������ �����ϴ�.
	
	// �����Ϳ� ���� ������� ù��° ��ҿ� ���� ������ �غý��ϴ�.
	StatInfo* monster_0 = monsters;
	monster_0->hp = 100;
	monster_0->attack = 10;
	monster_0->defence = 1;

	// �׷��ٸ� �ι�°... �׸��� ���� ��ҵ��� ��� �����ұ��?
	// �������� ��������� �̿� �����Ϳ� +1 �� Ÿ���� ũ�⸸ŭ�� �����Դϴ�.
	StatInfo* monster_1 = monsters + 1;
	monster_1->hp = 200;
	monster_1->attack = 20;
	monster_1->defence = 2;

	// �����Ϳ� ������ ��������� ��ȯ �����ϴ� 
	StatInfo& monster_2 = *(monsters + 2);
	monster_2.hp = 300;
	monster_2.attack = 30;
	monster_2.defence = 3;

	// [����] �̰Ŵ� ���� �ٸ��ǹ̴�
	// &�� �پ��ֳ� �� �پ� �ֳ� �������� �� �ڵ�� monsters+2 �� �ּҿ� �ִ� ������ �����ؼ� 
	// ������ ������ �����
	StatInfo temp = *(monsters + 2);
	monster_2.hp = 300;
	monster_2.attack = 30;
	monster_2.defence = 3;

	// �ݺ����� ����� �ڵ�ȭ�Ѵ�!
	for (int i = 0; i < 10; i++)
	{
		// ���!) ���� �����̸��� ��� ����� �� �ִ� ������ �ݺ� ���ึ�� ������ ��ȿ������ �����Ƿ� �ٽ� ����� �� �ֽ��ϴ�. 
		StatInfo& monster = *(monsters + i);
		monster.hp = 100 * (i + 1);
		monster.attack = 10 * (i + 1);
		monster.defence = (i + 1);
	}
	

	// �ٵ� *(monsters + i) �ʹ� �����ϰ� �������� �������� ... �� ���� ���?
	// �ε���! 
	// �迭�� 0������ ����. N��° �ε����� �ش��ϴ� �����Ϳ� �����Ϸ��� �迭�̸�[N]
	// *(monsters = i) = monsters[i]

	// �̷��� �����ϰ� �迭�� ��ҵ鿡 ���� �� �� �ֽ��ϴ�.
	monsters[0].hp = 100;
	monsters[0].attack = 10;
	monsters[0].defence = 1;

	for (int i = 0; i < 10; i++)
	{ 
		monsters[i].hp = 100 * (i + 1);
		monsters[i].attack = 10 * (i + 1);
		monsters[i].defence = (i + 1);
	}
	// ���� ���� ����
	int a = monsters[0].hp;

	// �迭 �ʱ�ȭ ���� ���
	int numbers[5] = {};  // 0���� �ʱ�ȭ
	int numbers1[10] = { 1, 2, 3, 4, 5 };  // ������ �ֵ��� ������ �����, ������ ������ 0�� �ʱ��
	int numbers2[] = { 1, 2, 3, 4, 5, 6 };  // ������ ���� ��ŭ�� ũ�⿡ �ش��ϴ� �迭�� ������ش�.
	char hellostr[] = { 'H', 'e', 'l', 'l', 'o', '\0' };
	cout << hellostr << endl;


	// �迭 ���> :
	// 1) �����Ѵ�
	int arr[10] = { };

	// 2)  �ε����� �����ؼ� ���
	arr[1] = 1;




	return 0;
}