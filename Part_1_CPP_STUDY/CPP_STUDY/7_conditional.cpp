#include <iostream>
using namespace std;

// ������ ���� : �б⹮
// �����͸� �޸𸮿� �Ҵ��ϰ� �����ϴ� ����� ���� �˾ƺ�
// ������ �����͸� �̿��ؼ� �����ΰ��� �ϰ� �ʹٸ�?

int main()
{
#pragma region �б⹮ if��
	int hp = 100; // ������ ü��
	int damage = 100; // �÷��̾� ������

	hp -= damage;  // �ǰ� ����
	bool isDead = (hp <= 0);  // óġ ����

	// ���Ͱ� �׾����� ����ġ �߰�
	// ����������� cmp, jmp

	// ��ǥ������ if��
	// if (����)   // �������� ������ �Ҹ��� ���� �;��ϴ� ���� �ƴմϴ�. 
	//	 ��ɹ�
	
	if (isDead)
		cout << "���͸� óġ �߽��ϴ�." << endl;	

	if (!isDead)  // Ȥ�� (isDead == false)
		cout << "���Ͱ� �ݰ� �߽��ϴ�." << endl;
	// if������ ���� ������ ������ �Ǻ��Ǿ� ����Ǹ� ��ǻ� ���� if���� ���� �ʾƵ� �� ���Դϴ�.
	// ������ ����ó�� if���� �ι� ���޾� ����� ������ ��� ������ �Ѵ� �Ǻ��� ������ ����ɷ��� ���� �ϰ� �˴ϴ�.

	if (isDead)
		cout << "���͸� óġ�߽��ϴ�." << endl;
	else
		cout << "���Ͱ� �ݰ��߽��ϴ�." << endl;
	// if else ������ ������ �Ǻ��ϸ� ù ���ǿ� ���ԵǸ� �ڵ����� ���� ��ɹ��� �н� �ϰ� �˴ϴ�.

	// if ������ �� ���̻��� ����� �ϰ� �ʹٸ� { }�� ����ؾ��մϴ�.
	if (isDead)
	{
		cout << "���͸� óġ�߽��ϴ�." << endl;
		cout << "���͸� óġ�߽��ϴ�." << endl;
	}

	// if ������ ������ �������ϰ��
	 if(true) 
	 {
	 }
	 else
	 {
		 if (true)
		 {
		 }
		 else
		 {
		 }
	 }
	 // �̷��� if else ���� ��ø ��ų ���� ������ �ڵ尡 ������� �������� �ſ� ���� ���� �˴ϴ�.

	 //�׷��� if - else if - else ���� ����մϴ�.

	 if (isDead) 
	 {
		 cout << "���͸� óġ�߽��ϴ�." << endl;
	 }
	 else if (0 < hp <= 20)
	 {
		 cout << "���Ͱ� �������� �ֽ��ϴ�." << endl;
	 }
	 else 
	 {
		 cout << "���Ͱ� �ݰ��߽��ϴ�." << endl;
	 }
#pragma endregion

#pragma region switch case break ��
	 const int ROCK = 0;
	 const int PAPER = 1;
	 const int SCISSORS = 2;

	 int input = ROCK;

	 if (input == ROCK)
		 cout << "������ �½��ϴ�." << endl;
	 else if (input == PAPER)
		 cout << "���� �½��ϴ�." << endl;
	 else if (input == SCISSORS)
		 cout << "������ �½��ϴ�." << endl;
	 else
		 cout << "�� ���̴ϱ�?" << endl;

	 // if else ���� ȿ���� ������ ������ �ڵ尡 ������� �ݺ��� ��� �ǰ� ��Ÿ�� ������� �ֽ��ϴ�.


	 // switch - case  - break ��
	 // �������� ���������� ���ɻ����δ� if else ���� ����մϴ�.
	 // switch�� ���ڷ� ���� �迭�� ���� �� �ֽ��ϴ�. (float, string �迭�� �ȵ˴ϴ�.)

	 switch (input)
	 {
	 case ROCK:
		 cout << "������ �½��ϴ�." << endl;
			break;
	 case PAPER:
		 cout << "���� �½��ϴ�." << endl;
			break;
	 case SCISSORS:
		 cout << "������ �½��ϴ�." << endl;
			break;
	 default:
		 cout << "�� ���̴ϱ�" << endl;
		 break;
	 }
#pragma endregion
}