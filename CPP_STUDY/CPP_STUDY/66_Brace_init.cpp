#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

// ������ ���� : �߰�ȣ �ʱ�ȭ { }

/*
	C++ 11 ���� ���ο� �ʱ�ȭ ����� �߰��Ǿ����ϴ�.
	
	������ �����ϸ鼭 
	TYPE ����{ data };
	��� ���·� �ʱ�ȭ �� �� �ֽ��ϴ�.

	�� ���� �̷� �ʱ�ȭ ������ ���� �߰� �������?
	C++ ���� �������� �������ϴٰ� �ٸ� ���鿡�� �����ؼ� �������� ��찡 �ֽ��ϴ�. 
	�׸��� ��� �߰�ȣ �ʱ�ȭ�� �迭�� �ʱ�ȭ �Ҷ��� ����ϴ�. 
	int a[] = { 1, 2, 3, 4, 5 };
	�̷��� �迭������ ���鼭 �ٸ� ���������� �����°� Ǯ����ٴ� �����Դϴ�.

	���� vector�� �ʱ�ȭ �Ҷ�
	vector<int> v1; 
	v1.push_back(1); 
	���� �ϳ��� �����͸� �ʱ�ȭ ���ְų�
	vector<int> v2(10, 1);
	���� 1�̶�� ������ 10���� ���� ä�� �����ϰ� �߽��ϴ�.

	�߰�ȣ �ʱ�ȭ�� �߰� �ǰ�������
	vector<int> v3{ 1, 2, 3, 4 };
	�̷��� ����Ҽ� �ְԵǾ����ϴ�.


	�߰�ȣ �ʱ�ȭ�� ���� ���
	
	1) vector�� �����̳ʿ� �� ��︰��.

	2) ��� ��ȯ ����.
	- int ������ double ������  = �����ϰԵǸ� �����ϰ� �˾Ƽ� ������ �Ǿ����ϴ�.
	������ �߰�ȣ �ʱ�ȭ�� �ϰԵǸ� ��Һ�ȯ�� ������ �˴ϴ�. ������ ������ �ս��� ���°� �������ݴϴ�.

	3) Bonus Ŭ���� ������ ��Ȯ��
	Knight k(); 
	��� �⺻�����ڸ� �̿��� ��ü�� �����Ϸ��� �Ͽ����� ��� �� ������ �Լ��� �����ϴ� �����Դϴ�.
	��ȯ�� Knight�� �Լ��̸��� k ���ڿ� ������ ���� �Լ����� 
	Knight k;
	�̰� �⺻�����ڷ� ������ ��ü�Դϴ�.
	�׷��� 
	Knight k(10); 
	���� ���� �־ �����ڸ� ȣ���ϴٰ� �ް����� k();�� �����Ҷ��� �����̴ϴ�.
	�̷��� �߰�ȣ �ʱ�ȭ ������ �̿��ϸ� 
	Knight k{ };
	�̰� ������ �⺻ �����ڸ� �̿��� ��ü �����Դϴ�.


	�׷��� �׻� �߰�ȣ �ʱ�ȭ�� ����ϴ°� �ּ��ΰ�?
	�ƴմϴ� � ��쿡�� �߰�ȣ �ʱ�ȭ�� ����ϴ°� ���ذ� �Ǳ⵵ �մϴ�.
	�߰�ȣ�� ������ ���ڸ� �޴� �������� ��� initializer_list�� �޴� �����ڸ� ȣ���ϰ� �Ǵµ�
	�쿬�� ���� ������ ���ڸ� �޴� �����ڰ� ������� �׻� initializer_list�� �޴� �����ڰ� �켱�ǰ� �˴ϴ�.

	�̷� ���� ���������� ������ ��ǥ������ vector�� ���� ��� �ְڽ��ϴ�.
	vector<int> v(10, 2); 
	�� �������� �ǹ̴� 2��� �����͸� 10�� �������ִ� ��ü v�� �����Ѵٴ� ���Դϴ�.
	������
	vector<int> v{ 10, 2 };
	�� 10�� 2��� �����͸� ���� ���� ��ü v�� �����մϴ�.

	�̷��� �ǵ��ߴ� ������ �۵����� ���� �� �ֽ��ϴ�.


	�׷��� �ΰ��� ����� ���ϰ� ������ �ϸ� �ǰڽ��ϴ�.

	1) ��ȣ �ʱ�ȭ ( ) �� �⺻���� ����
	- �������� C++ (�źΰ��� ����)
	- vector�� Ư�� ���̽����� { } ���

	2) �߰�ȣ �ʱ�ȭ { }�� �⺻���� ����
	- �ʱ�ȭ ������ ��ġȭ
	- ��� ��ȯ ����
*/

class Knight
{public:
	Knight(int a, int b)
	{
		cout << "Knight(int, int)" << endl;
	}

	Knight(initializer_list<int> li)
	{
		cout << "Knight(initializer_list<int>)" << endl;
	}
public:
	int _hp;
};
int main()
{
	// �ʱ�ȭ ������
	int a = 10;
	int b(0);
	int c{ 0 };

	// Ŭ������ ���
	Knight k1;
	Knight k2 = k1;  // ���� ������ (���� ������ X)

	Knight k3;
	k3 = k1;		// �̰� ���� ������

	// �߰�ȣ �ʱ�ȭ Ŭ���� ����
	Knight k4{ k2 };

	// vector���� �����̳ʿ����� ���
	vector<int> v1{ 1, 2, 3, 4, 5 };


	// �߰�ȣ �ʱ�ȭ�� ����
	// 1) vector�� �����̳ʿ� �߾�︰��.
	
	// 2) ��� ��ȯ ����
	int x = 0;
	//double y{ x };

	// 3) Bonus
	Knight k5{ };


	// �߰�ȣ �ʱ�ȭ�� ����? ������?
	// �߰�ȣ�ȿ� ������ ���ڰ� ���� �������� ��� initializer_list��� Ÿ���� �޴� �����ڰ� ȣ���� �˴ϴ�.
	// ������ ���� Ư�� ������ ���ڸ� �޴� �����ڰ� ���� �ִ°�쿡��?
	
	Knight k6{ 1, 2 };
	// int �ΰ��� �޴� �����ڿ� initializer_list�� �޴� ������ �Ѵ� ������ initializer_list�� �켱�˴ϴ�.

	
	

	return 0;
}