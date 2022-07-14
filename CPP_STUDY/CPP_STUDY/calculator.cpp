#include <iostream>
using namespace std;

// ������ ���� : �������� ����
// �����͸� �����ϴ� ����� ���� �˾ƺ��ڽ��ϴ�.

// a[ 1 ] 
// a��� �̸��� �ٱ��ϸ� �Ҵ��ϰ� �ȿ� 1�� �ִ´�.
int a = 1;

// b[ 2 ]
// b��� �̸��� �ٱ��ϸ� �Ҵ��ϰ� �ȿ� 2�� �ִ´�.
int b = 2;

int hp = 100;
bool isInvincible = true;

bool isSame;
bool isDifferent;
bool isGreater;
bool isSmaller;

bool test;

int main()
{
#pragma region ���������
	// ��� ������
	
	
	// ���� ������
	// a�� b�� �����ϰ� b�� ��ȯ�϶�.
	// -> b ��� �ٱ��� �ȿ� �ִ� ���� a ��� �ٱ��� �ȿ� �����ض�.
	a = b;


	// ��Ģ ������
	// ���� �ʿ��Ѱ�?
	// ex) ������ ���
	// ex) ü���� ��´ٰų�
	// ex) ���������� ī���͸� 1 ���� ��Ų�ٰų�

	a = b + 3;  // ���� add
	a = b - 3;  // ���� sub
	a = b * 3;  // ���� mul
	a = b / 3;  // ������ div
	a = b % 3;  // ������ div

	a += 3;  // a = a + 3; �� ����
	a -= 3;
	a *= 3;
	a /= 3;
	a %= 3;

	// ���� ������
	a = a + 1;  // ���������  add eax, 1   =   inc eax
	a++;  
	++a;
	a--;
	--a;

	b = a++;  // b = a; -> a�� 1����
	b = ++a; //  a�� 1���� -> b = a;

	b = (a + 1) * 3;  // ���а� ���������� ������ �켱�ϹǷ� ��ȣ�� ������ ����ؾ��մϴ�.

#pragma endregion

#pragma region	�񱳿���

	// ���� �ʿ��Ѱ�? 
	// ex) ü���� 0�� �Ǹ� ���ó��
	// ex) ü���� 30% ���ϰ� �Ǹ� �ñر⸦ �ߵ� (100 * hp / maxhp)
	// ex) ����ġ�� 100 �̻��̸� ������

	// a == b;	: a �� b �� ���� ������?
	// ������ 1, �ٸ��� 0
	isSame = (a == b);

	// a != b;  : a�� b�� �ٸ���?
	isDifferent = (a != b);

	// a > b : a�� b���� ū��?
	// a >= b : a��  b���� ũ�ų� ������?
	isGreater = (a > b);

	// a < b
	// a <= b
	isSmaller = (a < b);
#pragma endregion

#pragma region ������
	// ���� �ʿ��ұ�? ���ǿ� ���� ���� ��� �ʿ��� ��
	// ex) �α��� �� �� ���̵� ���� ��й�ȣ�� ���ƾ� �Ѵ�.
	// ex) ��� ������ OR ��� ��� �̸� ��� �ػ��� ����.

	// ! not
	// 0 �̸� 1�� ��ȯ, �� �ܴ� 0�� ��ȯ
	test = !isSame;  // ��ǻ� isDifferent �� �ǹ�

	// &&	and
	// a && b -> �Ѵ� 1�̸� 1, �� �ܿ��� 0
	test = (hp <= 0 && isInvincible == false); // ����

	// ||	or
	// a || b -> �� �� �ϳ��� 1�̸� 1 (�Ѵ� 0�̸� 0)
	test = (hp > 0 || isInvincible == true);

#pragma endregion
}