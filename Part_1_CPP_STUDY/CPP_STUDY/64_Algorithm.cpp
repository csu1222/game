#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
using namespace std;

#include <algorithm>

// ������ ���� : �˰���

/*
	�̹��ð����� ������ Ǯ���� ���������� ���� ����Ǵ� ������� �þ�ڽ��ϴ�.

	���α׷����� ���ٺ��� �� �ΰ��� �� ���� �����Դϴ�.
	
	�ڷᱸ�� (�����͸� �����ϴ� ����)
	�˰��� (�����͸� ��� ����� ���ΰ�?)

	C++���� �����ϴ� �˰��� ���̺귯���� �� ���� �ʹ� ���Ƽ� ��� �˰����� ���� �����ϸ鼭
	����� �ʿ�� ������ ���� ����ϴ� �˰������ �� �˾Ƶθ� �������Դϴ�.

	find
	find_if
	count
	count_if
	all_of
	none_of
	for_each
	remove
	remove_if


	 ���� 1) number��� ���ڰ� ���Ϳ� �ִ��� üũ�ϴ� ��� (bool, ù ���� iterator)
	 find ��� �˰����� ����ϸ� �����ϰ� ����� ����Ҽ� �ֽ��ϴ�.
	 find�� ���ڸ� 3���޴µ��� ù��° ���ڷ� ���� ���ͷ����Ϳ� �ι�° ���ڷ� ���� ���ͷ����� ���̿��� 
	 ����° ���ڷ� ���� ���� ã���ݴϴ�. ��ȯ�� ã�� ���� ���ͷ������Դϴ�.

	 ���� 2) 11�� ������ ���ڰ� ���;ȿ� �� �ִ��� üũ (bool, ù ���� iterator)
	 find_if��� �˰������� ���� �����ϰ� �����Ҽ� �ֽ��ϴ�.
	 find ó�� ù��° �ι�° ���ڷ� ã�� ������ �����ϰ�, ����° ���ڷ� � �������� ã���������� �ָ� �˴ϴ�.
	 ������ �Լ�ó�� ()�����ڸ� ����Ҽ� �־���ϰ� ���ڷ� ã�� ���� �ִ� �ñ״��Ŀ��� �մϴ�. 
	 struct CanByDivid11
		{
			bool operator() (int n)
			{
				return (n % 11) == 0;
			}
		};
	�̷��� �Լ���ü�� �����Ͽ��� 
	find_if(v.begin(), v.end(), CanByDivid11());
	�̷��� �ָ� �ݹ� �Լ��� ���ڴ� �˾Ƽ� ������ ��ȸ�ϸ鼭 �־��� ���Դϴ�.


	���� 3) Ȧ���� ������ ������? (count)
	count�� count_if�� �������� count�� � �� ��ü�� ���� ���̰�, count_if�� � ������ �����ϴ� ������ ���� ���Դϴ�.
	�� find�� find_if ó�� ����ϸ� �ǰڽ��ϴ�. �ٸ����� ��ȯ���� find���� ���ͷ����Ͱ� count���� int��� ���Դϴ�.

	�߰��� ����ϰ� ���Ǵ� �͵��� �� �Ұ� �ϸ�
	
	��� �����Ͱ� (����) �Դϱ�?
	std::all_of(begin(), end(), (����));
	
	�������߿� (����)�� �ϳ��� �ֽ��ϱ�?
	std::any_of(begin(), end(), (����));
	
	��� �����Ͱ� (����)�� �ƴմϱ�?
	std::none_of(begin(), end(), (����));


	���� 4) ���Ϳ� �� �ִ� ��� ���ڵ鿡 3�� �����ּ���
	for_each�� ����մϴ�. for_each�� ���� ó�� �� �����͸��� � ���� �ϴµ��� ��������, �׳� ��� �����͸� ��ȸ�ϴµ����� 
	���˴ϴ�. 

	
	ó���� ���ø� �� �Լ����� remove, remove_if�� ���ҽ��ϴ�. �� ���� �ٸ� �Լ���� ���� �ٸ��� �����Ѵٰ� �մϴ�.


	���� 5) Ȧ���� �����͸� �ϰ� �������ּ���
	remove_if�� ����ؾ��մϴ�. ���²� ����� �ٷδ� �ٷ� �ҵ� ����Ҽ� ������ ������ �� ����� ���� ���� �̻��� ���Ͱ� 
	���ɴϴ�. ���÷� 1, 3, 4, 6, 7, 8 �̶�� �����͸� ���� ���Ϳ��� 
	std::remove_if(����.begin(), ����.end(), IsOdd());
	�̷��� �־����ϴ�. �� ��� ���ʹ� 4, 6, 8, 6, 7, 8 �̶�� ���� ������ �Ǿ����ϴ�. �� ������ remove �� �Լ����� �˾Ƽ� 
	���Ϳ��� ���ǿ� ���� �ʴ� ������� ����� ���� �ƴ� ���ǿ� �´� �����͵��� ���� ���Ϳ� �����س��� ���̾����ϴ�. 
	�� ������ ���Ͱ� �߰� �� ������ �ϸ� �� �������� ��ü �����͸� �̵��ؾ� ������ �ݴ�� ���ǿ� �����ʴ�, ���ܳ������ 
	�����͸� �ϳ��� ���� �س��°��� �� ������ �����Դϴ�. �����͸� ���������� ���ͷ����͸� ��ȯ�մϴ�.
	�׷��� ���ϴ� ����� ��� ���ؼ��� remove, remove_if���� ���� erase�� ���� �����͵��� �����ָ� �ǰڽ��ϴ�.
	����.erase(std::remove_if(����.begin(), ����.end(), IsOdd()), ����.end());


*/

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

		vector<int>::iterator itFind = find(v.begin(), v.end(), number);
		if (itFind == v.end())
		{
			cout << "�� ã����" << endl;
		}
		else
		{
			cout << "ã����" << endl;
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

		struct CanByDivid11
		{
			bool operator() (int n)
			{
				return (n % 11) == 0;
			}
		};

		vector<int>::iterator itFind = find_if(v.begin(), v.end(), CanByDivid11());
		if (itFind == v.end())
		{
			cout << "�� ã����" << endl;
		}
		else
		{
			cout << "ã����" << endl;
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

		struct IsOdd
		{
			bool operator() (int n)
			{
				return (n % 2) != 0;
			}
		};

		int cnt = count_if(v.begin(), v.end(), IsOdd());

		cout << "count_if = " << cnt << endl;
	}

	// ���� 4) ���Ϳ� �� �ִ� ��� ���ڵ鿡 3�� �����ּ���
	{
		for (int i = 0; i < v.size(); i++)
		{

			v[i] *= 3;
		}

		struct MultipleBy3
		{
			void operator() (int& n)
			{
				n *= 3;
			}
		};

		std::for_each(v.begin(), v.end(), MultipleBy3());
	}

	// ���� 5) Ȧ���� �����͸� �ϰ� �������ּ���
	{
		/*for (vector<int>::iterator it = v.begin(); it != v.end();)
		{
			int num = *it;
			if ((num % 2) != 0)
				it = v.erase(it);
			else
				++it;
		}*/

		// �����͸� ���캸�� ���� ���߷� ���ڽ��ϴ�.
		v.clear();

		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(7);
		v.push_back(10);
		v.push_back(8);

		// remove�� � ���� ����
		//std::remove(v.begin(), v.end(), 2);

		// remove_if�� �������� ����

		struct IsOdd
		{
			bool operator() (int n)
			{
				return (n % 2) != 0;
			}
		};

		vector<int>::iterator it = std::remove_if(v.begin(), v.end(), IsOdd());
		v.erase(it, v.end());

	}

	return 0;
}