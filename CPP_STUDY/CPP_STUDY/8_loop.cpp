#include <iostream>
using namespace std;

// ������ ���� : �ݺ���
// ������ ������ ū ���ٱ� �б⹮, �ݺ���



int main()
{
	// while -���ȿ�
	// if-else ������ �����ϴ�
	// ������ �� ���� �����ϴ°� �ƴ϶�, Ư�� ���Ǳ��� �ݺ��ؾ� �ϴ� ���
	// ex) ������ �� ������ ��� ������ �����ض�
	// ex) �������� ���� �� ������ ��� �̵��϶�
	cout << "while��" << endl;

	int count = 0;

	while (count < 5)
	{
		cout << "Hello World" << endl;
		count++;
	}

	// do-while ��
	// while ������ ���̴� do �κ��� �ڵ带 �ѹ��� �����ϰ� �� �� while �κп��� ������ üũ�ϹǷ�
	// ���ǿ� ���յ��� �ʴ��� �ѹ��� ����˴ϴ�.

	cout << "do-while��" << endl;

	count = 0;
	do
	{
		cout << "Hello World" << endl;
		count++;
	} while (false);


	// for��  
	// while ������ count�� ������ �ϴ� ������ �ݺ��� �ۿ��� �ʱ�ȭ�ǰ� 
	// ī��Ʈ�� ������Ű�� �ڵ嵵 ������ ��ġ�� ���� ������ 
	// ���� �ڵ� ��Ÿ�Ͽ� ���� �ؼ��ϱ� �߰��� �� �ֽ��ϴ�.
	// for�� ������ �ʱ��, ���ǽ�, ������� ��� �� �������� �����ϴ�.

	cout << "for��" << endl;

	for (int i = 0; i < 5; i++)
	{
		cout << "Hello World" << endl;
	}


	// break; continue;
	// ���� ���� � ������ �Ǹ� �ݺ����� ���������ų�
	// ���� ���� � ������ �Ǹ� ���� �ڵ带 �����ϰ� ���� ������ ���� �ϰ� ������


	cout << "break ��" << endl;

	int round = 1;
	int hp = 100;
	int damage = 10;

	// ���� ���� : ���� ����
	while (true)
	{
		hp -= damage;
		if (hp < 0)
			hp = 0; // ���� ü���� ����

		// �ý��� �޼���
		cout << "Round " << round << " ���� ü�� " << hp << endl;

		if (hp == 0)
		{	
			cout << "���� óġ!" << endl;
			break;
		}

		if (round == 5)
		{
			cout << "���� ���� ����" << endl;
			break;
		}
		round++;
	}
	
	// 1~ 10 ������ Ȧ���� ����ϱ�
	// continueȰ��
	
	for (int num = 1; num <= 10; num++)
	{
		bool isEven = ((num % 2) == 0);

		if (isEven)
			continue;	


		cout << num << endl;
	}

}