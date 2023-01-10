#include <iostream>
using namespace std;

// ������ ���� : �ζ� ��ȣ ������


// 1) ����
void Swap(int& a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

	//	���� �ǽ� (���� ������ ����)
//void Sort(int numbers[], int count)
//{
//	int temp;
//	for (int i = 0; i < count; i++)
//	{
//		for (int j = i; j < count; j++)
//		{
//			if (numbers[j] < numbers[j + 1])
//				continue;
//			else
//			{
//				Swap(numbers[j], numbers[j + 1]);
//			}
//		}
//	}
//}


// 2) �����ϱ�
void Sort(int numbers[], int count)
{
	for (int i = 0; i < count; i++)
	{
		// i ��°�� ���� ���� �ĺ���� ����
		int best = i;
		
		// �ٸ� �ĺ��� �񱳸� ���� ���� ���� �ĺ��� ã�� ������
		for (int j = i + 1; j < count; j++)
		{
			if (numbers[j] < numbers[best])
				best = j;
		}

		// ���� ���� �ĺ��� ��ü�ϴ� ����
		if (i != best)
			Swap(numbers[i], numbers[best]);
	}
}

// 3) �ζ� ��ȣ ������
void ChooseLotto(int numbers[])
{
	// TODO : �������� 1~45 ������ ���� 6���� ����ּ���! (��, �ߺ��� ����� ��)
	int count = 0;
	while (count < 6)
	{
		int landValue = 1 + (rand() % 45);

		// �̹� ã�� ������?
		bool found = false;
		for (int i = 0; i < count; i++)
		{
			if (numbers[i] == landValue)
			{
				// �̹� ã�� ��
				found = true;
				break;
			}
		}

		// �� ã������ �߰�!
		if (found == false)
		{
			numbers[count] = landValue;
			count++;
		}
	}

	Sort(numbers, 6);
}

void ChooseLotto2(int numbers[])
{
	for (int count = 0; count != 6;)
	{

		int landValue = 1 + (rand() % 45);

		// �̹� ã�� ������?
		bool found = false;
		for (int i = 0; i < count; i++)
		{
			if (numbers[i] == landValue)
			{
				// �̹� ã�� ��
				found = true;
				break;
			}
		}

		// �� ã������ �߰�!
		if (found == false)
		{
			numbers[count] = landValue;
			count++;
		}
	}
	Sort(numbers, 6);
}


int main()
{
	srand((unsigned)time(0));
	// 1) Swap �Լ� ����� 
	int a = 1;
	int b = 2;
	cout << "���� �� a:" << a << " b:" << b << endl;
	Swap(a, b);
	cout << "���� �� a:" << a << " b:" << b << endl;

	// 2) ���� �Լ� ����� (���� ���ڰ� ���� ������ ����)
	int numbers[] = { 1, 42, 3, 15, 5, 6 };
	Sort(numbers, sizeof(numbers) / sizeof(int));

	// 3) �ζ� ��ȣ ������
	ChooseLotto2(numbers);

	for (int i = 0; i < (sizeof(numbers) / sizeof(int)); i++)
	{
		cout << "|" << numbers[i] << "|" << endl;
	}

	return 0;
}