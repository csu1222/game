#include <iostream>
using namespace std;

// ������ ���� : �ݹ� (Callback)

/*
	�ݹ�(Callback) : �ٽ� ȣ���ϴ�? ������ ȣ���ϴ�? (��ȭ �������� call me back �� ����� �ǹ�)

	������ ���� �� �̷� �ݹ��� ������ ���� �����Ѵ�.
	ex)MoveTack �ǽ� ��

	� ��Ȳ�� �Ͼ�� -> �� ����� ȣ������
	ex) UI ��ų ��ư�� ������ -> ��ų�� ���� �Լ��� ȣ������

	�ݹ� �Լ��� �Լ� ������ ���۶� ���÷� ����������ϴ�. 
	������ Ŭ������ �ְ� (������ ���̵�, ��͵�, ������ ���̵� ��� ������ ������ �ִ�)
	������Ŭ������ �迭�� ����� �� �����۵��� ���ǿ� �´� (�ݹ�) �������� ã�� �Լ��� ���÷� ������ϴ�.
	�׶��� �� ���ǿ� ���� ����� �Լ��� ������ ����°� ���� ����� �κ��� ���� ����� ���ǿ� �ش��ϴ� �κ���
	�Լ� �����ͷ� �ʿ信���� �ٲ㛧���ϴ�.

	�׸��� �� �������� �Լ��������� �������� ���¸� �����Ҽ� ���� �Ȱ��� �ñ״��ĸ� ���� �Լ��鸸 ��밡�� �߾����ϴ�.
	�� �κ��� �������� ���Ŀ� ��� �Լ���ü(Functor), ���ø����� �ذ��Ҽ� �־����ϴ�.


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
		// TODO : ���� üũ
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