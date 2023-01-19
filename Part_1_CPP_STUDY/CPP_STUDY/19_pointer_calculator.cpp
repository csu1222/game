#include <iostream>
using namespace std;

// ������ ���� : ������ ����

// 1) �ּ� ������ (&)
// 2) ��� ������
// 3) ���� ������
// 4) ���� ��� ������

struct Player // ���� ��� ������ ����
{
	int hp;
	int damage;
};

int main()
{
	int number = 1;

	// 1) �ּ� ������ (&)
		// - �ش� ������ �ּҸ� �˷��ּ��� -
		// - �� ��Ȯ�� ���ϸ� �ش� ���� Ÿ��(TYPE)�� ���� TYPE* ��ȯ
		int* pointer = &number;

	// 2) ��� ������ (+ -)

		// ������ ���� ��������ڸ� �� �̻� �پ��� ������� ��� �߾����ϴ�.
		number = number + 1;
		number++;
		++number;
		number += 1;

		// �����Ϳ��� �Ȱ��� ����� �� �ֽ��ϴ�.
		pointer = pointer + 1;
		pointer++;
		++pointer;
		pointer += 1;

		// ������ ������ ��������� ���� Ư���� ���� �ֽ��ϴ�.
		/*
		���� ��� 
		number = number + 1;

		pointer = pointer + 1;

		�� ���� ���Ҷ� ���� int ���� number�� �Ϲ������� �����ϵ� ���� 1�� ���� �߽��ϴ�.
		�ݸ�, ������ pointer�� �޸𸮿��� ��� +1 �� �ϴ� number�� ����Ű�� �ּҰ����� 
		4�� �����߽��ϴ�(?)

		�� ������ �����͸� ���� �� �� 
		int* pointer = &number;		��� ���� �߾����ϴ�. ���� int* �� �ٽ� ���캸�ڸ�
		- * : �� ������ ������ Ÿ���̴�! (8����Ʈ ũ��) �ּҸ� ��� �ٱ���!
		- int : �ּҸ� ���󰡸� int(4����Ʈ ������ �ٱ���)�� �ִٰ� �����ϰ� �м��ض�!
	
		[!] �����Ϳ��� +�� -�� ��� �������� 1�� ���ϰų� ����,
		���� '�� ����'�� ���ϰ� ����� �ǹ̰� �ƴϴ�!
		�ѹ��� TYPE�� ũ�⸸ŭ �̵��϶�!
		����/���� �ٱ��Ϸ� �̵��ϰ� �ʹ� << [�ٱ��� ����]�� �̵�����
		��, 1�� ���ϸ� = �ٱ��� 1�� ũ�� ��ŭ �̵����Ѷ�
		3�� ���ϸ� = �ٱ��� 3�� ũ�� ��ŭ �̵����Ѷ�
		*/
	
	// 3) ���� ������ (*)
		// ������ �����ϱ⸦ ��Ż�� Ÿ�� �ش� �ּҷ� ��~ �̵��Ѵٰ� �߽��ϴ�.
		* pointer = 3;  // number = 3;  �� ���� �ǹ�

	// 4) ���� ��� ������ (->)

		// ������ ����ü(struct)�� ����� �������� ���� ���� �߾����ϴ�.
		Player player;
		player.hp = 100;
		player.damage = 20;
		
		// �����͸� ����ü���� ������ �� �ֽ��ϴ�.
		Player* playerPtr = &player;

		// �׷��� ���������ڸ� ����� ���ڽ��ϴ�.
		(*playerPtr).hp = 200;
		(*playerPtr).damage = 50;
		// ��������󿡼� ��� �ᱹ *playerPtr �� �ΰ��� ���빰�� hp�� + 0 �� �ּҿ� �ִٴ°� �� �� �ְ�
		// damage�� +4 �� ��ġ�� �ִٴ°� �˼� �־����ϴ�.

		// �� ������ ��ȣ�� *�̳� . �� ���ļ� �����ϰ� ǥ���ϴ°� 
		// ���� ��� ������ (->) �Դϴ�.
		playerPtr->hp = 300;
		playerPtr->damage = 100;
		// ���� ������ ���� �ǹ��Դϴ�.
		// * �� ���� �����ڷ� (��ŻŸ�� �ش� �ּҷ� GO GO)
		// . �� ����ü�� Ư�� ����� �ٷ궧 ��� (����������� ���� ��ǻ� ������ ����)

	
	return 0;
}