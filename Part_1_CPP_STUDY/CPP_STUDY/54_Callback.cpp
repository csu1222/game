#include <iostream>
using namespace std;

// 오늘의 주제 : 콜백 (Callback)

/*
	콜백(Callback) : 다시 호출하다? 역으로 호출하다? (영화 같은데서 call me back 과 비슷한 의미)

	게임을 만들 때 이런 콜백의 개념이 자주 등장한다.
	ex)MoveTack 실습 등

	어떤 상황이 일어나면 -> 이 기능을 호출해줘
	ex) UI 스킬 버튼을 누르면 -> 스킬을 쓰는 함수를 호출해줘

	콜백 함수를 함수 포인터 시작때 예시로 만들었었습니다. 
	아이템 클래스가 있고 (아이템 아이디, 희귀도, 주인의 아이디를 멤버 변수로 가지고 있는)
	아이템클래스의 배열을 만들고 이 아이템들중 조건에 맞는 (콜백) 아이템을 찾는 함수를 예시로 들었습니다.
	그때는 각 조건에 따라 비슷한 함수를 여러개 만드는것 보다 공통된 부분을 같이 만들고 조건에 해당하는 부분을
	함수 포인터로 필요에따라 바꿔썻습니다.

	그리고 그 다음으로 함수포인터의 단점으로 상태를 저장할수 없고 똑같은 시그니쳐를 가진 함수들만 사용가능 했었습니다.
	이 부분의 단점들을 이후에 배운 함수객체(Functor), 템플릿으로 해결할수 있었습니다.


*/

class Item
{
public:

public:
	int _itemId = 0;
	int _rarity = 0;
	int _ownweId = 0;
};

class FindByOwnerId
{
public:
	bool operator() (const Item* item)
	{
		return (item->_ownweId == _ownerId);
	}
public:
	int _ownerId;
};

class FindByRarity
{
public:
	bool operator() (const Item* item)
	{
		return (item->_rarity >= _rarity);
	}
public:
	int _rarity;
};

template<typename T>
Item* FindItem(Item items[], int itemCount, T selector)
{
	for (int i = 0; i < itemCount; i++)
	{
		Item* item = &items[i];
		// TODO : 조건 체크
		if (selector(item))
			return item;
	}

	return nullptr;
}

int main()
{
	Item items[10];
	items[3]._ownweId = 100;
	items[8]._rarity = 2;

	FindByOwnerId functor1;
	functor1._ownerId = 100;

	FindByRarity functor2;
	functor2._rarity = 1;

	Item* item1 = FindItem(items, 10, functor1);
	Item* item2 = FindItem(items, 10, functor2);

	return 0;
}