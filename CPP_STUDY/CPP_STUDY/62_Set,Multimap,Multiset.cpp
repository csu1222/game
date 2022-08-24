#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
using namespace std;

// ������ ���� : set, multimap, multiset

/*
	map �̿��� ���� �����̳ʵ�
	set, multimap, multiset

	set : set�� map�� �������� map�� <key, value> �� ���� �����ϴ� �����̳ʿ�����
		set�� key�� �����ϴ� �����̳ʶ�� �� �� �ֽ��ϴ�. key������ key���� �� value�� ����ϴ� ���Դϴ�.

		set�� ������ map�� ���� �����մϴ�.
		�ְ�	(insert)
		����	(erase)
		ã��	(find)
		�ݺ���	(set<int>::iterator) (*it)

		map�� ���� �ٸ����� �ִٸ� []�����ڸ� �������� �ʴ´ٴ°� []�� key�� value���� ���� �ִ� map�̶� �����ϴ°��̾���
		key = value�� set�� []�����ڰ� �ʿ����� �ʽ��ϴ�.


	multimap, multiset : �� ���� �̹� ��� map�� set���� �ߺ� key�� ����ϴ� �����̳��Դϴ�. 
		key, value�� ���¸� ����ϴ°� ��ü�� ���� key�� value���� ã�� ���� �ϱ����ؼ� ����ϴ� ���̶� 
		�ߺ� key�� ����ϴ� multi �ø��� ���� ����ϴ°� �� ���� ���̱� ������ �ϴ� � �������� ������ڽ��ϴ�.

	multimap : ��κ� map�� �����ϰ� �ٸ����� key�� �ߺ������ �ȴٴ� ���Դϴ�. �׷��ٺ��� erase�� � key�� ���ﶧ 
		���� ��尡 �ѹ��� ������ ���� �ְ� ���� �ϳ��� ��常 ��� ����� �ʹٸ� find�� ���� key���� ���� ����� �� ó��
		��带 �����ٰ� ++ -- ���� �����ڷ� ���� ��带 ��� erase�� �� �� �ֽ��ϴ�. 
		find�� �ϳ��� ���°� �ƴ� ���� key�� ���� ������ ���� ������ equal_range�� �ش� ������ ���ͷ����͸� pair�� 
		��ȯ�Ҽ� �ֽ��ϴ�. pair�� �ƴ϶� �� ���ͷ����͸� ���ϸ� lower_bound, upper_bound�� ���ͷ����͸� ������ �ֽ��ϴ�.

	multiset : multimap�� ���� �Ȱ����ϴ�. key���� ������ multimap�̶�� �����ϸ� �ǰڽ��ϴ�.
*/
#include <set>

int main()
{
	// set
	// key = value
	set<int> s;
	s.insert(10);			// �ְ�
	s.insert(30);			
	s.insert(60);			
	s.insert(90);			
	s.insert(20);			
	s.insert(40);

	set<int>::iterator findIt = s.find(10);				// ã��
	
	s.erase(10);			// ����
	
	set<int>::iterator it;	// �ݺ���
	for (set<int>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << *it << endl;
	}

	cout << "--------------------------------------------------" << endl;

	// multimap
	// �ߺ� key�� ����ϴ� map

	multimap<int, int> mm;

	mm.insert(make_pair(1, 100));		// �ְ�
	mm.insert(make_pair(1, 200));		
	mm.insert(make_pair(1, 300));		
	mm.insert(make_pair(2, 400));		
	mm.insert(make_pair(2, 500));

	multimap<int,int>::iterator findIt2 =  mm.find(1);							// ã��	
	// find�� key�� �ش��ϴ� ù iterator�� ��ȯ�մϴ�.
	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> itPair;
	itPair = mm.equal_range(1);
	// equal_range�� key�� �ش��ϴ� iterator���� ù iterator�� pair�� first�� ,
	// ������ iterator�� ���� iterator�� second�� �ݴϴ�.
	for (multimap<int, int>::iterator it = itPair.first; it != itPair.second; ++it)
	{
		cout << it->first << " " << it->second << endl;
	}
	// equal_range�� ��ȯ�ϴ� pair�� �ѹ��� �����ϴ°� ���� ���� �����ϰ� �ʹٸ�
	multimap<int, int>::iterator lower = mm.lower_bound(1);	// equal_range �� pair�� first�� �ش�
	multimap<int, int>::iterator upper = mm.upper_bound(1);	// equal_range �� pair�� second�� �ش�

	unsigned int count;
	count = mm.erase(1);					// ����
	// erase�� key�� �ش��ϴ� ��� ��带 �����ϰ� ������ ī��Ʈ�� unsigned int�� ��ȯ�մϴ�.
	cout << "erase count " << count << endl;

	// multimap<int, int>::iterator;		// �ݺ���
	// ��� ������ ��ȸ
	for (multimap<int, int>::iterator it = mm.begin(); it != mm.end(); ++it)
	{
		cout << it->first << " " << it->second << endl;
	}


	cout << "--------------------------------------------------" << endl;

	multiset<int> ms;

	// �ְ�
	ms.insert(100);
	ms.insert(100);
	ms.insert(100);
	ms.insert(200);
	ms.insert(200);

	// ã��		
	multiset<int>::iterator msFindIt = ms.find(100);

	pair<multiset<int>::iterator, multiset<int>::iterator> msItPair;

	msItPair = ms.equal_range(100);
	multiset<int>::iterator mslower = ms.lower_bound(100);
	multiset<int>::iterator msupper = ms.upper_bound(100);

	for (multiset<int>::iterator it = msItPair.first; it != msItPair.second; ++it)
	{
		cout << *it << endl;
	}

	for (multiset<int>::iterator it = mslower; it != msupper; ++it)
	{
		cout << *it << endl;
	}



	return 0;
}