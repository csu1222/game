#include <iostream>
using namespace std;

// ������ ���� : �Լ� (���ν���, �޼ҵ�, ��ƾ)

/*
* �Լ��� ��
input���� ������ �ް� output���� ������ ������ �����ش�

��ȯŸ�� �Լ��̸�(����Ÿ��(�Ű�����))
{
	�Լ� ����

	return ~~
}

*/

// Hello World�� �ֿܼ� ����ϴ� �Լ��� ����� ����
// input : ���� / output : ����
// Ÿ�� : int, float, double, char~ , ��ȯ�ϴ°� ���� void

void PrintHelloWorld()
{
	cout << "Hello World" << endl;
}

// ������ �Է�(�Լ�) �޾Ƽ�, �ֿܼ� ����ϴ� �Լ��� ����� ����.
// input : int  // output : ����
void PrintNumber(int number)
{
	cout << "�Ѱ��ֽ� ���ڴ� " << number << " �Դϴ�." << endl;
}

// 2�� ���ϴ� �Լ��� ������
// input : int // output : int

int MultiplyBy2 (int a) 
{
	int b = a * 2;
	return b;
}

// �� ���ڸ� �ް� ���� �� ���ڸ� ���� �� ��� �Լ�
// input : int, int // output : int

int MultiplyBy(int a, int b)
{
	int c = a * b;
	return c;
}

// ���� : ����� ����Ű
// F5 : �ߴ��� ���� ���� (���� �ߴ����� ���ٸ� ����� ����)
// F10: ���ν��� ���� ���� , ���ν����� �Լ��� �ٸ� �̸��Դϴ�. ���� �� ����׸� �ϴٰ� �Լ��� ������ �Լ� ������ ������
// - ���� �ʰ� �����ٷ� ����˴ϴ�.
// F11: �Ѵܰ� �� ���� �� �� �� ����׸� �ϴٰ� �Լ��� ������ �� �Լ��ȿ� ���� �� ���پ� �����մϴ�.

int main()
{
	int result = MultiplyBy(3, 5);
	PrintHelloWorld();
	PrintNumber(result);
}