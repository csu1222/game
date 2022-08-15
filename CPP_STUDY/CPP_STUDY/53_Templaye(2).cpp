#include <iostream>
using namespace std;

// ������ ���� : ���ø� ����

/*
	���ø� : �Լ��� Ŭ������ ���� Ʋ
	1) �Լ� ���ø�
	2) Ŭ���� ���ø�  # 
	
	Ŭ���� ���ø��� �Լ� ���ø��� ����ϰ� template<typename T> �� Ÿ�� ������ �̸� ���� 
	Ÿ�Ժκп� ���ø��� ���ָ� �˴ϴ�. ����Ҷ� �Լ����� ���� �ٸ����� 
	�Լ��� ����Ҷ� ���ڰ� �ٷ� ���� ��������� Ÿ�� ������ �����൵ ������
	Ŭ���� ������ ��� ���ڸ� �־��ִ� �����ڰ� �ƴ� �׳� �����ϴ� Ÿ���� ��� ��������� 
	� Ÿ���� Ŭ������ ��������� �˷���� �ڽ��ϴ�.

	�׷��� ������ typename�� �ٿ��� �ϴ� ���� �ƴϴ�?
	typename�� ���������� ���Ǵ°��� �ƴ϶� ���� RandomBoxŬ������ ��� �迭�� ũ�⵵ ���������� ���������ϴ�.
	templae< > �ȿ� ���°� [������ �ϴ� ���]�̶�� �� �� ���� 
	template<typename T, int SIZE>  �̷��� Ÿ�Գ��Ӱ� SIZE��� int���� ���ø����� ���� 
	�迭�� �� Ÿ��, �迭�� ũ�⸦ ���� T, SIZE�� ���־����ϴ�. 
	�� Ŭ���� ���ø��� ����
	RandomBox<int, 10> rbx1;  �̷� ������ Ÿ�԰� �迭�� ũ�⸦ �������մϴ�.

	�׷��� Ŭ���� ���ø����� ���� Ŭ������ � ���ڸ� �޴� �Լ����� ��������
	�Լ��� ���� ���ڷ� ������ ȣ���ϴ°��� �ϳ��� �Լ��ּҷ� ���ڸ� �ٲ�鼭 ����Ǵ°��̰�
	Ŭ���� ���ø����� ���ø��� �ٲ㰡�鼭 ȣ���ϴ°��� �� �ѹ� ȣ�⶧���� ���ο� Ŭ������ �����ϴ� ���Դϴ�.

	�Լ� ���ø� ������ ��� ���ø� Ư��ȭ�� Ŭ���� ���ø����� ��� �� �� �ֽ��ϴ�. 
	���ø� Ư��ȭ�� �ٽ� �������ڸ�, � Ư�� Ÿ������ ���ø��� ����ϸ� �������� ������ Ŭ������ ����� �ִ� ���Դϴ�.
	���ø� Ư��ȭ ������ �Լ� ���ø����� ����ϱ⵵ �ϰ� ���� �ٸ��⵵ �մϴ�.
	template<int SIZE>		 �Լ� ���ø��� ó�� typename�� ����ݴϴ�. int SIZE�� Ư��ȭ�� ������ �׳� ���ϴ�.
	
	class RandomBox<double, SIZE>
	{	
	������
	}
	�Լ� ���ø� Ư��ȭ�� �ٸ��� Ŭ���� �̸��� � Ÿ���� ���� Ư��ȭ�� �ɰ����� �ñ״��ĸ� ��������մϴ�.
*/

template<typename T, int SIZE>
class RandomBox
{
public:
	T GetRandomData()
	{
		int idx = rand() % SIZE;
		return _data[idx];
	}

public:
	T _data[SIZE];
};

// ���ø� Ư��ȭ
template< int SIZE>
class RandomBox<double, SIZE>
{
public:
	double GetRandomData()
	{
		cout << "randomdox double" << endl;
		int idx = rand() % SIZE;
		return _data[idx];
	}

public:
	double _data[SIZE];
};

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	RandomBox<int, 10> rb1;

	for (int i = 0; i < 10; i++)
	{
		rb1._data[i] = i;
	}
	int value1 = rb1.GetRandomData();
	cout << value1 << endl;

	RandomBox<double, 20> rb2;

	for (int i = 0; i < 20; i++)
	{
		rb2._data[i] = i + 0.5;
	}
	double value2 = rb2.GetRandomData();
	cout << value2 << endl;


	return 0;
}