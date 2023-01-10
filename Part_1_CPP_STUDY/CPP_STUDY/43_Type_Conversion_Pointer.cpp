#include <iostream>
using namespace std;


// 오늘의 주제 : 타입 변환 (포인터)

#pragma region 복습
/*
복습)
클래스를 지역변수로 생성하는 것과 동적 할당으로 생성하는 것의 차이
- 지역변수로 : 변수의 데이터가 전부 스택에 위치합니다. 변수생성시 자동으로 생성자가 실행되고
	유효 범위가 끝날때도 자동으로 소멸자가 실행됩니다.
- 동적 할당으로 : 변수는 포인터입니다. 주소만 스택에 저장됩니다. 객체의 데이터는 힙 영역에 위치합니다.
	변수 생성시 자동으로 생성자가 실행되지만, delete하지 않고 유효범위가 끝날시 스택의 포인터만 해제되고
	힙 영역의 객체는 해제되지 않습니다. (유효범위가 끝나기 전 delete를 해주어야 합니다.)
클래스를 인자로 받는 함수, 클래스의 포인터를 인자로 받는 함수
- 클래스를 인자로 : 이전에 배웠듯이 이 경우는 매개변수로 클래스를 주고 함수내로 가져갈때 지역변수로 복사해 가져갑니다.
	그 복사를 하는 순간 클래스는 다시 생성자함수를 실행하고 함수내에 작업이 끝나면 소멸자 함수가 호출됩니다.
- 포인터를 인자로 : 이 경우에는 매개변수로 준 클래스를 포인터로 가져다가 쓰는 것이기 때문에 원래 있던 클래스를 외부에서
	 조작하는 느낌입니다.

객체의 배열과 객체 포인터 타입 배열

객체의 배열 : 말그대로 배열 각 인덱스에 객체들이 있습니다. 배열 생성시 정한 크기만큼 있습니다.

객체 포인터의 배열 : 배열 생성시 정한 크기만큼의 인덱스에 객체를 가리킬 포인터들이 있습니다. 하지만 객체 자체는 없고
	각 인덱스에는 그저 포인터만 있습니다. 객체를 설정 해주려면 for문을 돌아 각 인덱스에 new문을 해줍니다.
	이후 delete도 for문으로 해주어야 합니다.

*/
#pragma endregion

/*
[1] 연관이 없는 클래스 사이의 포인터 변환
	특징) 암시적으로는 불가능, 명시적으로는 가능합니다. 하지만 항상 조심해야 합니다. 엉뚱한 메모리를 건드릴 수 있기 때문

[2] 상속 관계의 클래스 사이의 포인터 변환
	[2-1] 부모 -> 자식 으로 포인터 변환
	- 연관없는 클래스 사이의 포인터 변환 처럼 암시적으로 불가, 명시적으로 가능합니다.
	[2-2] 자식 -> 부모 사이의 포인터 변환
	- 암시적으로도 가능! 자식이 상속받은 범위 까지만 타입 변환 되므로 가능

명시적으로 타입 변환할 때는 항상 항상 조심해야 한다!
암시적으로 될 때는 안전하다?
-> 평생 명시적으로 타입 변환(캐스팅)은 안 하면 되는거 아닌가?
	하지만 프로젝트에서 본래 타입에서 캐스팅하고 다시 돌아오고 하는 작업들이 필요합니다. 
	그러니 그런 기능을 구현할 때는 정신 똑바로 차리고 해야 메모리가 엉망이 되지 않을것입니다.

	****** 중요 ******
클래스를 포인터 변환 해 사용 했다고 치고 결국 delete를 해주어야 할 것입니다. 클래스가 여러개면 반복문을 돌아가면서
delete해주어야 할것이고요 
이러 저러 해서 delete를 해줄 때 만약 부모 클래스로 타입변환된 상태에서 delete 해주게 되면 부모의 소멸자 함수가 불려옵니다.
해결은 다시 조건문으로 이 객체가 타입 변환 되기 전에 어떤 클래스였는지 확인 후 본래 클래스로 타입변환, 그리고 delete를 해주어야 합니다.

이런 문제를 이전에 배웠던 적이 있는데, 상속을 배울 때였습니다. 소멸자 함수가 아니더라도 멤버 함수에 오버라이딩시 외부에서 이 
멤버 함수를 호출하게 되면 최상위 부모의 멤버 함수가 불렸습니다. 
이 문제를 해결 해줬던 문법이 virtual, 이번 클래스 끼리의 포인터 변환에서도 적용 할 수 있습니다.
최상위 무모 소멸자를 virtual, 가상함수로 만들어 주게되면 알아서 본래 클래스의 소멸자를 호출하게 됩니다.



 [ 결론 ]
 - 포인터 vs 일반 타입 : 차이를 이해하자
 -- 객체를 일반적으로 생성 vs new 문법으로 포인터와 Heap영역을 사용
 -- 함수의 인자로 일반타입을 줘서 매개변수 복사해서 사용 vs 함수의 인자로 포인터를 줘서 원본에 접근하기
 - 포인터 사이의 타입 변환 (캐스팅)을 할 때는 매우 매우 조심해야 한다!
 -- 위험함에도 불구하고 부모 - 자식 클래스 간의 타입변환을 사용해야 할 때 가 있습니다. 조심해서 사용!
 - 부모-자식 관계에서 부모 클래스 소멸자에 까먹지 말고 virtual을 붙이자!! ** 면접에서 자주나온다고 합니다. ** 
 -- 이유는 클래스 상속에서의 오버라이딩 문제를 떠올리자! (객체지향 다형성에서 다뤘습니다.)
*/
enum ItemType
{
	IT_Weapon = 1,
	IT_Armor = 2,
};

class Knight
{
public:
	int _hp = 0;
};


class Item
{
public:
	Item()
	{
		cout << "Item()" << endl;
	}

	Item(const Item& Item)
	{
		cout << "Item(const Item&)" << endl;
	}

	Item(int itemType) : _itemType(itemType)
	{
		cout << "Item(int itemType)" << endl;
	}

	virtual ~Item()
	{
		cout << "~Item()" << endl;
	}
public:
	int _itemType = 0;
	int _itemDbId = 0;

	char _dummy[4096] = {};  // 이런 저런 정보들로 비대해진걸 묘사
};

class Weapon : public Item
{
public:
	Weapon() : Item(IT_Weapon)
	{
		cout << "Weapon()" << endl;
		_damage = rand() % 100;
	}
	~Weapon()
	{
		cout << "~Weapon()" << endl;
	}
public:
	int _damage = 0;
};

class Armor : public Item
{
public:
	Armor() : Item(IT_Armor)
	{
		cout << "Armor()" << endl;
		_defence = rand() % 10;
	}
	~Armor()
	{
		cout << "~Armor()" << endl;
	}
public:
	int _defence = 0;
};

void TestItem(Item item){ }

void TestItemPtr(Item* item){ }

int main()
{
#pragma region 복습
	// 복습
	{
		// stack [ type(4) dbid(4) dummy(4096) ]
		Item item;

		// stack [ 주소(4~8) ] -> heap  [ type(4) dbid(4) dummy(4096) ]
		Item* item2 = new Item();

		// 복사생성자 실행
		TestItem(item);
		TestItem(*item2);

		// 인자에 직접 접근
		TestItemPtr(&item);
		TestItemPtr(item2);

		// delete하지 안으면 메모리 누수 (Memory Leak) -> 점점 가용 메모리가 줄어서 크래시
		delete item2;
	}

	// 배열
	{
		cout << "----------------------절 취 선----------------------" << endl;
		// 진짜 아이템이 100개 있는것 (스택 메모리에 올라와 있는)
		//Item item3[100] = {};

		cout << "----------------------절 취 선----------------------" << endl;
		// 아이템이 100개 있을까요?
		// 아이템을 가리키는 바구니가 100개, 실제 아이템은 1개도 없을 수도 있다
		Item* item4[100] = {};

		// item4에 실제 item 객체를 만들어 주려면?
		cout << "----------------------절 취 선----------------------" << endl;
		for (int i = 0; i < 100; i++)
		{
			item4[i] = new Item;
		}
		cout << "----------------------절 취 선----------------------" << endl;
		for (int i = 0; i < 100; i++)
		{
			delete item4[i];
		}
	}
#pragma endregion

	// 연관성이 없는 클래스 사이의 포인터 변환 테스트
	{
		// Stack [ 주소 ] -> Heap [ _hp(4) ]
		Knight* knight = new Knight();

		// 암시적으로는 NO
		// 명시적으로는 OK
		// Stack [ 주소(knight) ]
		/*Item* item = (Item*)knight;
		item->_itemType = 2;
		item->_itemDbId = 3;

		delete knight;*/

	}

	// 부모 -> 자식 포인터 변환 테스트
	{
		Item* item = new Item();

		// 암시적으로는 NO
		// 명시적으로는 OK
		//Weapon* weapon = (Weapon*)item;
		//weapon->_damage = 5;

		//delete item;
	}
	
	// 자식 -> 부모  포인터 변환 테스트
	{
		Weapon* weapon = new Weapon();

		// 암시적으로도 OK
		Item* item = weapon;

		delete weapon;

	}

	// 명시적으로 캐스팅을 해야  할 때
	// 20칸 인벤토리에 랜덤으로 아머와 무기를 채워넣기
	Item* inventory[20] = {};
	srand((unsigned int)time(0));

	for (int i = 0; i < 20; i++)
	{
		int randValue = rand() % 2;

		switch (randValue)
		{
		case 0:
			inventory[i] = new Weapon();
			break;
		case 1:
			inventory[i] = new Armor();
			break;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		Item* item = inventory[i];
		if (item == nullptr)
			continue;

		if (item->_itemType == IT_Weapon)
		{
			Weapon* weapon = (Weapon*)item; // 이런 명시적 포인터 타입변환은 무서운 것이지만. 지금은 원래 데이터 타입으로 변환 하는 것이라 가능
			cout << "Weapon damage :" << weapon->_damage << endl;
		}
		if (item->_itemType == IT_Armor)
		{
			Armor* armor = (Armor*)item;
			cout << "Armor defence : " << armor->_defence << endl;
		}
	}
	// ***************** 매우 중요 ************************

	for (int i = 0; i < 20; i++)
	{
		Item* item = inventory[i];

		if (item == nullptr)
			continue;
		//if (item->_itemType == IT_Weapon)
		//{
		//	Weapon* weapon = (Weapon*)item; // 이런 명시적 포인터 타입변환은 무서운 것이지만. 지금은 원래 데이터 타입으로 변환 하는 것이라 가능
		//	cout << "Weapon damage :" << weapon->_damage << endl;
		//	delete weapon;
		//}
		//else
		//{
		//	Armor* armor = (Armor*)item;
		//	cout << "Armor defence : " << armor->_defence << endl;
		//	delete armor;
		//}
		delete item;
		
	}
	

	



	return 0;
}