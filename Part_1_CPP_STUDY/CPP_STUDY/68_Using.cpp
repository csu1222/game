#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>


// ������ ���� : using


/*
	C++ 11 ���� ���� typedef ��� ����ϰԵ� �����Դϴ�.

	�⺻���� 
	typedef TYPE NAME;	// ���𹮹��� ����
	using NAME = TYPE;  // ���� ������ ���� ����

	typedef �� using�� ���ϸ�
	
	1) ������ (�Լ������͸� ���ε��)
	typedef - ���𹮹����� �Լ��̸��ڸ��� NAME�� �ֽ��ϴ�.
	using 	- ������ �ñ״��İ� �����̶�� �̸����� ���ڴٰ� ǥ���մϴ�.

	2) ���ø�
	typedef�� tempalte�� ���� ������� ���մϴ�.
	- C++11 �������� struct�� class�ȿ� �ٽ� typedef�� ����� ���������� ����߽��ϴ�.
	using�� template�� ��밡���մϴ�.


	��� : using�� typedef���� ���鿡�� ��뼺�� ����.
		�׷��� ���� typedef�� ����ϴ� �ڵ带 �������� �˾ƾ��Ѵ�

*/

// 1) ������
typedef void(*FUNC)();
using FUNC2 = void(*)();

// 2) ���ø�
template<typename T>
//typedef std::list<T> List; 
using List = std::list<T>;

// C++ 11 ����
template<typename T>
struct List2
{
	typedef std::list<T> type;
};


int main()
{

	// 2) 
	List<int> li;
	li.push_back(1);
	li.push_back(2);
	li.push_back(3);

	List2<int>::type li2;

	return 0;
}