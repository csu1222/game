#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
using namespace std;

// ���� ����

// ���� ������������ Ǯ��ôµ� �׶� �׶� § �� �ڵ���� ���� ������ �����¾����� �ּ��� �ƴմϴ�.
// �ٸ� ����� �� ó�� �ôٸ� �ƹ��� ������ �ڵ�� ���ʴ� �ڵ带 �ؼ��ϴ� �ð��� �ɸ� ���̰�
// �� ������ ������ vector�� �������� § �ڵ��̱� ������ �ٸ� �����̳ʸ� �����ؼ� ������ ȣȯ�� �����˴ϴ�.
// �׷��� C++���� �������� �����ϴ� ��ɵ��� ���ٸ� ��� C++ ���α׷��Ӱ� ����� ������ �ڵ带 �������ְ�
// �޾Ƶ��ϼ� ���� ���Դϴ�. 
// ���� ������ algorithm���� ������ڽ��ϴ�.
//

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// ���� 1) number��� ���ڰ� ���Ϳ� �ִ��� üũ�ϴ� ��� (bool, ù ���� iterator)
	{
		int number = 50;

		bool found = false;
		vector<int>::iterator checkIt = v.end();

		// TODO
		for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		{
			int value = *it;
			if (value == number)
			{
				found = true;
				checkIt = it;
				break;
			}
		}
	}

	// ���� 2) 11�� ������ ���ڰ� ���;ȿ� �� �ִ��� üũ (bool, ù ���� iterator)
	{
		bool found = false;
		vector<int>::iterator checkIt = v.end();


		for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		{
			int value = *it;
			if (value % 11 == 0)
			{
				found = true;
				checkIt = it;
				break;
			}
		}
		
	}


	// ���� 3) Ȧ���� ������ ������? (count)
	{
		int count = 0;

		for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		{
			int value = *it;
			if (value % 2 == 1)
			{
				count++;
			}
		}
	}

	// ���� 4) ���Ϳ� �� �ִ� ��� ���ڵ鿡 3�� �����ּ���
	{
		for (int i = 0; i < v.size(); i++)
		{
			
			v[i] *= 3;
		}
	}


	return 0;
}