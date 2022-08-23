#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <map>
using namespace std;

// ������ ���� : map - �� ����

/*
	���� �����̳�
	map : ���� ���� Ʈ�� (AVL)
	- ��� ������� �����ְ� �ڽź��� ũ�ų� ������ �Ǻ��Ͽ� �ΰ��� ����(����)�� �����͸� �����ϴ� �ڷᱸ��

	map�� �����ϴ� ���� _reft, _right��� �ٸ� �����ͷ� ���� Node* �� �ְ�,
	�����ϰ��� �ϴ� �����͸� �����ͷ� ������ �ְ�, �������� �� _key��� ���� ������ �ֽ��ϴ�.

	map�� ����Ҷ��� < > �ȿ� key�� Ÿ���� int, �׸��� value�� Ÿ���� �������־�� �մϴ�.

	��Ȳ1 �� 10���� �÷��̾ �����Ѵٰ� �ϸ� for���� 10���� ���鼭 
	map<int, int> m; ���� ������ m �� 
	m.insert(); �� map�� �־��ַ��� �մϴ�. �̶� insert�� ���ڷ� pair��� ����ü�� �޴µ� 
	�� pair�� �׳� �� Ÿ���� ��� �����ϴ� ����ü�Դϴ�. map������ key�� value�� pair�� ������ �ǰڽ��ϴ�.
	pair<int, int>(key, value); 

	��Ȳ2 �� �̹����� 5������ �÷��̾ �����Ѵٰ� �غ��ڽ��ϴ�. for������ 5������ ���鼭 
	randomVlaue�� rand() % 50000; �̶�� �������� �ְ� 
	m.erase(randomValue); �� ������ �÷��̾ ���� �Ͽ����ϴ�. 
	�̶� erase�� ���ڷ� ���������� �ü� �ִµ� key���� �޾� key ���� �ش��ϴ� ��带 ����ų�,
	���� ���ͷ����Ϳ� �� ���ͷ����͸� �޾� �� ������ ������ ����ų�,
	����� ���ͷ����͸� �޾� ��带 ����ų�, �׳� ���ͷ����͸� �޾� ����ϴ�.
	������ ���� ���� �Ϲ����� randomValue��� int���� key����� erase�ϰ� �ֽ��ϴ�.

	��Ȳ3 ���� ID�� 10000�� �÷��̾ ã��ͽ��ϴ�. map�� ����Ʈ���� ���ϴ� ��带 ã�°� �ſ� �����ٰ� �Ͽ����ϴ�.
	�и� map���� ���ϴ� ��带 ã�� ����� ������ ���� ã�ƺ���
	m.find() ��� ����� �ֽ��ϴ�. 
	find�� ���� ������ ������ �����ϰ� key���� �༭ ã�� ���ҽ��ϴ�.
	m.find(10000);
	�� find�� map�� iterator�� ��ȯ�մϴ�. ���� ã������ �� ��带 ����Ű�� iterator�� ��ȯ�ϰ� ã�� ��������
	m.end(); �� ��ȯ�մϴ�.

	��Ȳ4 ������ map�� ��ȸ�ϸ鼭 key�� value�� ������ �ͽ��ϴ�.
	vector�� deque�� [] �����ڷ� ��ȸ�ϸ鼭 ������ ������ �־����ϴ�.list ó�� iterator�� ++�����ڷ� ��ȸ�� �ϸ鼭 
	���� ������ �־����ϴ�.
	map�� iterator�� ����Ͽ� ��ȸ�� �ؾ��ϴµ�, []�������� ��� ����, ������ �ع����⶧���� �׳� ���� �ִ� �����͸�
	��ȸ�ϴµ����� �˸��� �ʽ��ϴ�.
	iterator�� ���� ������ ���� *�����ڸ� ����ϸ� pair�� ��ȯ�Ǵµ� first�� key�� second�� value
	�Դϴ�.

	ȣ������� erase�� ���� ��带 ������� �ι��̻� �ϸ� ��� ���� �ñ��մϴ�. 
	�켱 erase�� size_t�� ��ȯ�ϴµ� �� size_t�� unsigned int �� �����ϴ�. �� � ����� ��ȯ�ϴµ� �� ��ȯ���� erase�� 
	count��� �մϴ�. �׷��� ���� key�� �ι� erase�غ��鼭 ��ȯ���� ��� �Ǵ��� ��������� �˾ƺ��ҽ��ϴ�.
	ù��° erase������ ��ȯ���� 1�� ���Խ��ϴ�. �ι�° erase������ 0�� ���Խ��ϴ�. 
	�̸��� ���� ��󿡰� �ι��̻� erase�� �ص� �� ������ ���� �ٸ� count�� 0���� �� �ҿ뵵 ���ٴ� ���Դϴ�.

	�̹����� insert�� ���� Ű�� �ٸ� ������ �ι� �Ѵٸ�? 
	� Ű���� ����� ���� �ٲٰ� �; insert�� �� Ű�� �ٽ��ϸ� ���� ������ �����ؼ� ������ �غ��� 
	�ι�° insert�� ���õǰ� ó���� ���� �����Ǵ°� �˼� �ֽ��ϴ�.
	���� ��Ȯ�� Ȯ���Ϸ��� insert�� ��ȯ�� pair�� �� pair�� map::iterator, bool�� �̷���� �ִٰ� �մϴ�. �ش��ϴ� ������ �ϳ� ����
	�ι��� insert�� ���� ���������� ��������� ���ึ�� Ȯ���غ��� insert ù�õ��� ��ȯ���� insert�� �� �� ��ġ�� iterator�� 
	insert�� �� ����Ǿ��ٴ� true ���� ���� pair�� ��ȯ�˴ϴ�. �ι�° �õ��� ��ȯ���� insert�� �� iterator�� �����ߴٴ�
	false�� ��ȯ�ϰ� �˴ϴ�.

	�׷��� � Ű�� ã�Ƽ� ���ٸ� �߰�, �ִٸ� ������ �ϰ� ������ ��� �ؾ� �ɱ��
	ù��° ������� find�� � key�� ���� ��带 ã�� if������ �ִٸ� ã�� iterator�� �����ؼ� value�� �������ݴϴ�.
	���ٸ� insert�� ���ϴ� ���� �߰����ָ� �ǰڽ��ϴ�.
	�� ����� �� ���ŷο�� �ֽ��ϴ�. �׷��� �̹� ���� ����� �ֽ��ϴ� [] �������Դϴ�.
	�ι�°�� map[���ϴ� key��] = �߰�,�����ϰ� ���� value��;
	���� ���ٸ� �߰�, �ִٸ� ������ �� �� �ֽ��ϴ�. ������ ��ȸ������ ����ϱ� ����ġ �ʴٰ� �ߴµ� �̰�쿡�� �����մϴ�.


	���
	
	�ְ�	(insert, [])
	����	(erase)
	ã��	(find, [])
	�ݺ���	(map::iterator) (*it) pair<key, value>&
	4������ �� �˰� ����


*/

// ���ÿ� Ŭ����
class Player
{
public:
	Player() : _playerId(0) { }
	Player(int playerId) : _playerId(playerId) { }
public:
	int _playerId;
};

// map�� ������ Node class
class Node
{
public:
	Node*	_reft;
	Node*	_right;
	// DATA
	pair<int, Player*> _data;	// pair�� key�� value�� ����
	//int		_key;
	//Player* _value;
};
int main()
{
#pragma region  vector, list, deque ���� ������ �����̳��� ���� : Searching�� ������.
	// vector�� Player���� ���� �Ϸ��� �մϴ�
	vector<Player*> v;

	// ��Ȳ 1 10���� �÷��̾ ���ͼ� �� id�� �Ҵ��մϴ�.
	for (int i = 0; i < 100000; i++)
	{
		Player* player = new Player(i);
		v.push_back(player);
	}

	// ��Ȳ 2 ���Դ� �÷��̾��� ������ 5���� �����ϴ�.
	for (int i = 0; i < 50000; i++)
	{
		int randIndex = rand() % v.size();

		Player* p = v[randIndex];
		delete p;

		v.erase(v.begin() + randIndex);
	}

	// ��Ȳ 3  ID: 20000�� �÷��̾ ID: 10000�� �÷��̾ �����ϰ� �;��
	// Q) ID : 10000�� �÷��̾ ã������
	// A) vector���� �������� ������ �����ϴ� index�� �ܼ��� ���� �ʾƼ� ó������ �� �Ⱦ� ã�´�
	// ���� �÷��̾�� 5���̰� �־��� ��쿡 ���� ID �� 10000�� �÷��̾ ���ٸ� 5������ �������� ã�� ���ߴٴ� ����� ���ü� �ֽ��ϴ�.

	bool found = false;
	
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->_playerId == 10000)
		{
			found = true;
			break;
		}
	}

	// vector, list, deque ���� ������ �����̳��� ġ������ ����
	// -> ���ϴ� ���ǿ� �ش��ϴ� �����͸� ������ ã�� �� [����]

#pragma endregion

	// map<Key, Value> ����
	map<int, int> m;
	srand(static_cast<unsigned int>(time(nullptr)));

	// ��Ȳ1 : + 10����

	for (int i = 0; i < 100000; i++)
	{
		m.insert(pair<int, int>(i, i * 100));
	}

	// ��Ȳ2 : - 5����
	for (int i = 0; i < 50000; i++)
	{
		int randomValue = rand() % 50000;

		// Erase by Key
		m.erase(randomValue);
	}


	// Q) ID = 1���� Player�� ã�� �ʹ�!
	// A) �ſ� ������ ã���� ����

	map<int, int>::iterator findIt = m.find(10000);
	if (findIt != m.end())
	{
		cout << "ã��" << endl;
	}
	else
	{
		cout << "�� ã��" << endl;
	}

	// map ��ȸ 
	// map�� [ ]�� ������ iterator�� ��ȸ�ؾ��մϴ�.
	for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		pair<const int, int>& p = (*it);
		int key = p.first;
		int value = p.second;

		cout << key << " " << value << endl;
	}

	// �ִٸ� ����, ���ٸ� �߰��� ��� �ұ�? v1
	map<int, int>::iterator findIt2 = m.find(10000);
	if (findIt2 != m.end())
	{
		findIt->second = 200;
	}
	else
	{
		m.insert(make_pair(10000, 200));
	}

	// �ִٸ� ����, ���ٸ� �߰��� ��� �ұ�? v2
	m[5] = 500;
	
	return 0;
}