#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <map>
using namespace std;

// 오늘의 주제 : map - 빈도 높음

/*
	연관 컨테이너
	map : 균형 이진 트리 (AVL)
	- 노드 기반으로 균형있게 자신보다 크거나 작은지 판별하여 두가지 갈래(이진)로 데이터를 정리하는 자료구조

	map을 구성하는 노드는 _reft, _right라는 다른 데이터로 가는 Node* 가 있고,
	관리하고자 하는 데이터를 포인터로 가지고 있고, 데이터의 각 _key라는 값을 가지고 있습니다.

	map을 사용할때는 < > 안에 key의 타입인 int, 그리고 value의 타입을 지정해주어야 합니다.

	상황1 로 10만의 플레이어를 생성한다고 하면 for문을 10만번 돌면서 
	map<int, int> m; 으로 생성한 m 을 
	m.insert(); 로 map에 넣어주려고 합니다. 이때 insert가 인자로 pair라는 구조체를 받는데 
	이 pair는 그냥 두 타입을 묵어서 관리하는 구조체입니다. map에서는 key와 value를 pair로 묶으면 되겠습니다.
	pair<int, int>(key, value); 

	상황2 로 이번에는 5만명의 플레이어가 퇴장한다고 해보겠습니다. for문으로 5만번을 돌면서 
	randomVlaue로 rand() % 50000; 이라는 정수값을 주고 
	m.erase(randomValue); 로 랜덤한 플레이어를 삭제 하였습니다. 
	이때 erase의 인자로 여러가지가 올수 있는데 key값을 받아 key 값에 해당하는 노드를 지우거나,
	시작 이터레이터와 끝 이터레이터를 받아 그 범위의 노드들을 지우거나,
	상수형 이터레이터를 받아 노드를 지우거나, 그냥 이터레이터를 받아 지웁니다.
	지금은 그중 가장 일반적인 randomValue라는 int값을 key값삼아 erase하고 있습니다.

	상황3 으로 ID가 10000인 플레이어를 찾고싶습니다. map은 이진트리로 원하는 노드를 찾는게 매우 빠르다고 하였습니다.
	분명 map에서 원하는 노드를 찾는 기능이 있을것 같아 찾아보니
	m.find() 라는 기능이 있습니다. 
	find도 여러 버전이 있지만 간편하게 key값을 줘서 찾아 보았습니다.
	m.find(10000);
	이 find는 map의 iterator를 반환합니다. 만약 찾았으면 그 노드를 가리키는 iterator를 반환하고 찾지 못했으면
	m.end(); 를 반환합니다.

	상황4 현재의 map을 순회하면서 key와 value를 꺼내고 싶습니다.
	vector나 deque는 [] 연산자로 순회하면서 값을을 꺼낼수 있었습니다.list 처럼 iterator의 ++연산자로 순회를 하면서 
	값을 얻을수 있었습니다.
	map은 iterator를 사용하여 순회를 해야하는데, []연산자의 경우 수정, 생성을 해버리기때문에 그냥 원래 있는 데이터를
	순회하는데에는 알맞지 않습니다.
	iterator의 값을 꺼내기 위해 *연산자를 사용하면 pair가 반환되는데 first가 key고 second가 value
	입니다.

	호기심으로 erase를 같은 노드를 대상으로 두번이상 하면 어떻게 될지 궁금합니다. 
	우선 erase는 size_t를 반환하는데 이 size_t는 unsigned int 와 같습니다. 즉 어떤 양수를 반환하는데 이 반환값은 erase된 
	count라고 합니다. 그래서 같은 key를 두번 erase해보면서 반환값이 어떻게 되는지 디버깅으로 알아보았습니다.
	첫번째 erase에서는 반환값이 1이 나왔습니다. 두번째 erase에서는 0이 나왔습니다. 
	이말은 같은 대상에게 두번이상 erase를 해도 별 문제는 없고 다만 count가 0으로 별 소용도 없다는 말입니다.

	이번에는 insert를 같은 키에 다른 값으로 두번 한다면? 
	어떤 키값의 노드의 값을 바꾸고 싶어서 insert를 그 키로 다시하면 되지 않을까 생각해서 실제로 해보면 
	두번째 insert는 무시되고 처음의 값이 유지되는걸 알수 있습니다.
	좀더 정확히 확인하려면 insert의 반환이 pair고 이 pair는 map::iterator, bool로 이루어져 있다고 합니다. 해당하는 변수를 하나 만들어서
	두번의 insert에 각각 대입해준후 디버깅으로 시행마다 확인해보면 insert 첫시도에 반환값은 insert를 한 그 위치의 iterator와 
	insert가 잘 실행되었다는 true 값을 가진 pair가 반환됩니다. 두번째 시도의 반환값은 insert를 한 iterator와 실패했다는
	false를 반환하게 됩니다.

	그러면 어떤 키를 찾아서 없다면 추가, 있다면 수정을 하고 싶으면 어떻게 해야 될까요
	첫번째 방법으로 find로 어떤 key를 가진 노드를 찾고 if문으로 있다면 찾은 iterator에 접근해서 value를 수정해줍니다.
	없다면 insert로 원하는 값을 추가해주면 되겠습니다.
	위 방법이 좀 번거로울수 있습니다. 그래서 이미 좋은 기능이 있습니다 [] 연산자입니다.
	두번째로 map[원하는 key값] = 추가,수정하고 싶은 value값;
	으로 없다면 추가, 있다면 수정을 할 수 있습니다. 위에서 순회에서는 사용하기 적절치 않다고 했는데 이경우에는 적절합니다.


	요약
	
	넣고	(insert, [])
	빼고	(erase)
	찾고	(find, [])
	반복자	(map::iterator) (*it) pair<key, value>&
	4가지를 잘 알고 있자


*/

// 예시용 클래스
class Player
{
public:
	Player() : _playerId(0) { }
	Player(int playerId) : _playerId(playerId) { }
public:
	int _playerId;
};

// map을 구성할 Node class
class Node
{
public:
	Node*	_reft;
	Node*	_right;
	// DATA
	pair<int, Player*> _data;	// pair로 key와 value를 묶음
	//int		_key;
	//Player* _value;
};
int main()
{
#pragma region  vector, list, deque 같은 시퀀스 컨테이너의 단점 : Searching이 느리다.
	// vector로 Player들을 관리 하려고 합니다
	vector<Player*> v;

	// 상황 1 10만의 플레이어가 들어와서 각 id를 할당합니다.
	for (int i = 0; i < 100000; i++)
	{
		Player* player = new Player(i);
		v.push_back(player);
	}

	// 상황 2 들어왔던 플레이어중 절반인 5만이 나갑니다.
	for (int i = 0; i < 50000; i++)
	{
		int randIndex = rand() % v.size();

		Player* p = v[randIndex];
		delete p;

		v.erase(v.begin() + randIndex);
	}

	// 상황 3  ID: 20000인 플레이어가 ID: 10000인 플레이어를 공격하고 싶어요
	// Q) ID : 10000인 플레이어를 찾으세요
	// A) vector에서 랜덤으로 유저가 존재하니 index는 단서가 되지 않아서 처음부터 쭉 훑어 찾는다
	// 현제 플레이어는 5만이고 최악의 경우에 지금 ID 가 10000인 플레이어가 없다면 5만번의 루프끝에 찾지 못했다는 결과가 나올수 있습니다.

	bool found = false;
	
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->_playerId == 10000)
		{
			found = true;
			break;
		}
	}

	// vector, list, deque 같은 시퀀스 컨테이너의 치명적인 단점
	// -> 원하는 조건에 해당하는 데이터를 빠르게 찾을 수 [없다]

#pragma endregion

	// map<Key, Value> 생성
	map<int, int> m;
	srand(static_cast<unsigned int>(time(nullptr)));

	// 상황1 : + 10만명

	for (int i = 0; i < 100000; i++)
	{
		m.insert(pair<int, int>(i, i * 100));
	}

	// 상황2 : - 5만명
	for (int i = 0; i < 50000; i++)
	{
		int randomValue = rand() % 50000;

		// Erase by Key
		m.erase(randomValue);
	}


	// Q) ID = 1만인 Player를 찾고 싶다!
	// A) 매우 빠르게 찾을수 있음

	map<int, int>::iterator findIt = m.find(10000);
	if (findIt != m.end())
	{
		cout << "찾음" << endl;
	}
	else
	{
		cout << "못 찾음" << endl;
	}

	// map 순회 
	// map은 [ ]를 못쓰고 iterator로 순회해야합니다.
	for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		pair<const int, int>& p = (*it);
		int key = p.first;
		int value = p.second;

		cout << key << " " << value << endl;
	}

	// 있다면 수정, 없다면 추가는 어떻게 할까? v1
	map<int, int>::iterator findIt2 = m.find(10000);
	if (findIt2 != m.end())
	{
		findIt->second = 200;
	}
	else
	{
		m.insert(make_pair(10000, 200));
	}

	// 있다면 수정, 없다면 추가는 어떻게 할까? v2
	m[5] = 500;
	
	return 0;
}