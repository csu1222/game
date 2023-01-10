#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <algorithm>


// 오늘의 주제 : 람다 (lambda)

/*
	* 요약 
	* 1) 람다는 함수객체를 손쉽게 만드는 C++11 문법
	* 2) 람다 표현식은 [캡쳐](인자) { 구현부 }; 로 이루어져있다
	* 4) 캡쳐 모드에는 기본모드(=, &), 지정 모드(name, &name)이 있는데 지정 모드를 권장한다.
	* 3) 캡쳐가 있을때는 이 캡쳐된 데이터가 이미 해제된 메모리인지 유의한다.
	
	람다는 새로운 기능이 추가되는 것은 아니지만 기존 있던 문법을 더욱 빠르게 작성 할수 있게 해줍니다.

	람다를 한줄로 요약하면 "함수 객체를 빠르게 만드는 문법" 입니다.

	# 함수 객체 : 클래스(객체)지만 ()연산자 오버로딩으로 함수처럼 기능하는 객체

	람다는 결국 함수객체를 손쉽게 만드는 문법이니 함수객체의 예시로 예전에 배운 find_if 메소드를 예로 들어보겠습니다.
	예전 find_if 를 실습하기위해 Item클래스를 만들어서 멤버 변수로 아이디, 희귀도, 아이템 타입을 주었고 이 
	Item을 vector로 만들어 그중 조건에 맞는 아이템을 찾았었습니다.

	해당 아이템의 이터레이터 = std::find_if(시작 이터레이터, 끝 이터레이터, bool을 반환하는 함수객체);
	이런 형태였습니다. 세번째 인자로 함수객체가 들어가는데 예전에는 그때 필요한 함수객체를 새로 만들어서 넣어주었습니다.

	바로 세번째 인자로 넣어줄 함수객체를 생성하는 부분을 람다로 더욱 손쉽게 처리 할수 있다는 이야기 입니다

	----------------------------------------

	람다 표현식 (lambda expression)
	람다의 기본형태는 
	[]() {} 
	입니다. 생긴게 이상합니다. 
	람다는 익명함수(이름이 없는 함수)를 만듭니다. ()의 안에 매개변수를 넣고 {}안에 구현부를 넣으면 됩니다.

	struct IsUniqueItem
		{
			bool operator() (Item& item)
			{
				return item._rarity == Rarity::Unique;
			}
		};
	이런 아이템의 레어도가 유니크인 함수객체를 람다 표현식으로 표현 하면

	[](Item& item) { return item._rarity == Rarity::Unique; }
	이렇습니다. 아주 획기적으로 코드의 양을 줄여주었습니다.

	람다 표현식의 특이한 점은 위 함수객체에서 IsUniqueItem라고 이름을 지어주었던 부분이 없습니다. 진짜로 없는것은 아니고 
	컴파일러가 스스로 알아볼만한 이름을 임의로 지어줄것입니다. 또 한가지 특이한 점은 반환 타입이 생략되어있는데 
	반환 타입은 컴파일러가 return 부분을 보고 타입을 유추해줄 것입니다.

	혹시 자동 추론 타입말고 타입을 지정하고 싶다면
	[]() -> type { }
	type 부분에 반환 타입을 지정해주면 되겠습니다. 어지간하면 자동추론된 타입을 사용할것 같습니다.

	람다 표현식을 auto 타입으로 변수에 대입해주게되면 함수객체를 생성해준다고 했다시피 객체가 생성됩니다.
	이걸 클로저 (closure) 라고 합니다.

	클로저 (closure) : 람다에 의해 만들어진 실행시점 객체
	
	fint_jf문법에서 조건에 해당하는 인자는 함수 객체를 넣어줘야 햇는데요 람다를 어떤 변수에 담아 넣어줘도 되고 그냥
	람다 표현식 자체를 인자로 넣어도 됩니다. 
	딱히 따로 변수로 저장할 이유가 없다면 바로 인자로 넣어주는게 낫겠습니다.


	람다 표현식 []() { } 에서  [] 부분이 무엇인지 아직 설명하지 않았습니다.
	[] 부분을 캡쳐(capture)라고 하는데 
	캡쳐를 설명하기 앞서 함수 객체의 특징중에 상태를 가지고 있을 수 있다고 했습니다. 
	이 특징은 일반 함수와 달리 함수 객체는 멤버 변수로써 어떤 값을 가지고 있을 수 있고 그 값으로 함수 부분에서 사용 할수 있었습니다.
	다시 Item을 예시로 find_if를 사용해서 특정 id를 가진 아이템을 찾으려고 합니다. 이때 조건에 해당하는 함수 객체는
	멤버 변수로 아이템 아이디를 가지고 있고 생성자를 호출할때 그 아이디 값을 가져와 인자로 받은 아이템의 아이디와 비교합니다.

	이 함수 객체를 람다 표현식으로 하면 어떻게 될까요?
	int itemId = 4;
	[=](Item& item) { return item._itemId == itemId; }
	이렇게 표현이 됩니다. 함수객체 에서는 멤버 변수로 가지고 있던 찾고 싶은 아이디가 람다 외부에 있고 외부의  변수를 그냥 가져다 썼습니다.
	외부 변수를 어떤 방식으로 가져다 쓸지를 캡쳐( [ ] 부분 ) 에서 정해야 하는데요
	캡쳐의 기본 모드는 두 가지가 있습니다.

	캡쳐의 기본 모드 : 값(복사) 방식(=), 참조 방식(&)
	즉 [=] 와 [&] 두가지가 있습니다.
	
	값 복사 방식은 외부의 변수 값을 람다로 만든 객체 클로져에 복사하는 방식으로 가져옵니다. 복사하는 것이기 때문에 한번 값을 받은뒤에는 
	외부 변수의 값을 바꿔도 클로져에게는 영향이 없습니다.

	참조 방식은 말그대로 외부 변수 값을 참조로 클로져에 줍니다. 생성자의 인자를 참조형으로 만든것과 같은 이치 입니다. 참조형이다 보니
	클로져가 한번 값을 받은 뒤에도 외부 변수값이 클로져에게 영항을 줍니다.

	기본 캡쳐모드로 외부의 변수를 끌고 와 사용하는 것은 꼭 한개만되는 것도 아닙니다. 필요한 만큼 변수를 만들어 가져다 쓰면 되겠습니다.

	여러개를 캡쳐해 사용할때 모든 변수를 값방식, 참조방식으로 통일해서 사용한다면 불편할 것입니다. 값방식이 유용한 데이터도 있고, 
	참조 방식이 유용한 데이터도 있을 테니 말입니다.
	그래서 기본 캡쳐 모드는 모든 캡쳐에 대해 적용되는 캡쳐 모드고, 
	변수마다 캡쳐모드를 지정해서 사용할 수 있습니다. 대괄호 [ ] 안에 각 캡쳐할 변수의 이름을 그대로 넣거나, 앞에 &을 붙여 넣으면
	각각 캡쳐모드를 지정할수 있습니다.
	[name, &name]

	한번 지정하면 모든 캡쳐 모드를 적어야 하는것도 아닌 기본 캡처 모드를 지정하고 특정 캡쳐만 모드를 바꿀수도 있습니다.
	ex) [=, &name]

	보통 C++에서는 기본 캡쳐 모드를 지양 한다고 합니다. 
	조금 번거롭더라도 모든 캡쳐모드를 각각 지정해주는걸 권장 합니다. 
	왜냐하면 기본 캡쳐 모드는 어떤 캡쳐가 있는지 코드를 직접 뜯어봐야하고 
	개별로 지정해준다면 처음 코드를 작성할때 는 조금 귀찮지만 나중에 다른 사람이 이 코드를 본다면 [ ] 안에 어떤 캡쳐들이 있는지
	한눈에 알아 볼수 있기 때문입니다.


	그리고 람다 표현식이나 일반 함수 객체나 공통으로 유의할 점이 있는데 함수 객체가 상태를 가져다 쓰는것이나, 람다가 캡쳐를 사용할때 
	참조 타입으로 사용할 경우 참조된 원본 데이터가 날라가버린다면 컴퓨터가 어떻게 될지 아무도 모르게 됩니다.
	값 복사 타입의 경우는 참조 보다는 덜 하지만 그래도 어느정도 위험이 있습니다. 
	그 실습을 해보겠습니다.

	예시 상황은 Knight라는 클래스가 있고 int _hp = 100; 이라는 멤버 변수를 가지고 있습니다.
	Knight 클래스에 함수객체를 반환하는 멤버 함수를 만들었습니다. 이 멤버 함수로 만든 함수 객체를 필요할때 가져다 쓰려고 합니다.
	이 멤버 함수는 구현부에서 람다로 함수객체를 만들고 그 함수 객체를 return 합니다. 
	람다의 기능은 멤버 변수 _hp를 200으로 수정하는 기능입니다. 
	_hp에 빨간줄이 그어져서 위에서 배운 캡쳐 모드를 지정하지 않아서 그렇구나 싶어 &_hp 참조 타입으로 지정 했습니다. 
	그러자 이번에는 &_hp , _hp 둘 다 빨간줄이 그어집니다. 
	그 이유는 클래스 내부의 멤버 변수를 캡쳐할 경우 _hp라고 표현되지만 사실 this->_hp; 라는 자기 자신 포인터에서 멤버 변수를 
	꺼내는 방식이기 때문입니다. 그래서 캡쳐 모드를 [&] 또는 [this]를 사용해야 합니다. 
	이렇게 캡쳐모드까지 해결했고 
	Knight의 포인터 k에 new Knight(); 문법으로 동적할당을 했습니다.
	이후 auto job = k->ResetHpJob(); 로 _hp를 200으로 수정하는 함수 객체를 만들었습니다.

	위험한 상황은 job을 나중에 쓰려고 아껴두었다가 
	객체 k를 실수로 delete를 한뒤 job(); 으로 이미 delete 된 k의 _hp에 접근 하려고 했을 때입니다. 
	이미 날라간 메모리를 접근하게되고 이 버그는 크래쉬가 바로 나지 않을 수도 있어서 더욱 문제입니다.

	항상 이런 위험성을 인지하면서 사용해야합니다. 
	
	멤버 함수가 아닌 외부의 함수 객체로 사용할경우 함수객체의 상태로 Knight의 포인터를 들고 있고 
	구현부에서 포인터의 _hp로 접근해서 수저할 것입니다. 
	이 형태도 생성자를 호출할때 받은 Knight 포인터가 중간에 없어지거나 하면 위험하겠습니다.

	이런 메모리 오염 문제를 예방하는 차원에서도 캡쳐를 일괄 지정하지 않고 각각 지정해주면 
	캡쳐들이 수상하게 포인터 같다 싶으면 실수를 미리 막을 수 있습니다.

*/

// enum class 
enum class ItemType
{
	None,
	Armor,
	Weapon,
	Jewelry,
	Consumable,
};

enum class Rarity
{
	Common,
	Rare,
	Unique,
};

// 함수 객체 복습
class Item
{
public:
	Item() { }
	Item(int itemId, Rarity rarity, ItemType itemType) :
		_itemId(itemId), _rarity(rarity), _itemType(itemType)
	{

	}

public:
	int _itemId = 0;
	Rarity _rarity = Rarity::Common;
	ItemType _itemType = ItemType::None;
};





int main()
{
	vector<Item> v;

	v.push_back(Item(1, Rarity::Common, ItemType::Weapon));			// 겸사 겸사 push_back이 오른값 참조로 받는걸 알수 있습니다.
	v.push_back(Item(2, Rarity::Common, ItemType::Armor));
	v.push_back(Item(3, Rarity::Rare, ItemType::Jewelry));
	v.push_back(Item(4, Rarity::Unique, ItemType::Weapon));

	//  람다 = 함수 객체를 손쉽게 만드는 문법
	//  람다 자체로 C++에 '새로운' 기능이 들어간 것은 아니다.
	{

		// 지금까지 배운 내용들로 레어도가 유니크인 아이템을 찾는 기능을 구현 했습니다.
		struct IsUniqueItem
		{
			bool operator() (Item& item)
			{
				return item._rarity == Rarity::Unique;
			}
		};

		// 람다 표현식
		// auto IsUniqueLambda = [](Item& item) { return item._rarity == Rarity::Unique; };

		auto findIt = std::find_if(v.begin(), v.end(), [](Item& item) { return item._rarity == Rarity::Unique; });
		if (findIt != v.end())
			cout << "아이템 ID : " << findIt->_itemId << endl;

	}

	{
		// 함수 객체는 상태를 저장할수 있기었습니다.
		// 아이템 아이디를 받아서 해당하는 아이템을 찾는다.
		struct FindItem
		{
			FindItem(int itemId, Rarity rarity, ItemType type) : _itemId(itemId), _rarity(rarity), _type(type)
			{

			}

			bool operator() (Item& item)
			{
				return item._itemId == _itemId && item._rarity == _rarity && item._itemType == _type;
			}

			int _itemId;
			Rarity _rarity;
			ItemType _type;
		};
		
		int itemId = 4;
		Rarity rarity = Rarity::Unique;
		ItemType type = ItemType::Weapon;

		//  [ ] 캡처(capture) : 함수 객체 내부에 변수를 저장하는 개념과 유사
		// 사진을 찰칵 [캡처]하듯 ... 일종의 스냅샷을 찍는다고 이해
		// 기본 캡처 모드 : 값(복사) 방식(=), 참조 방식(&)

		auto FindItemByIdLambda = [=](Item& item) {	return item._itemId == itemId; };

		auto FindItemLambda = [=, &type](Item& item) { return item._itemId == itemId && item._rarity == rarity && item._itemType == type; };

		auto findIt = std::find_if(v.begin(), v.end(), FindItemLambda);
		if (findIt != v.end())
			cout << "아이템 ID : " << findIt->_itemId << endl;

	}


	{
		// 캡쳐, 함수 객체의 상태가 오염되었을때의 위험성
		class Knight
		{
		public:
			// 함수객체를 반환하는 멤버 함수
			auto ResetHpJob()
			{
				auto f = [this]()
				{ 
					this->_hp = 200;
				};
				return f;
			}
		public:
			int _hp = 100;
		};

		Knight* k = new Knight();

		// 함수 객체를 job 변수에 넣어서 필요할때 사용하려고 합니다.
		auto job = k->ResetHpJob();

		// 어쩌다가 객체 k를 삭제하게되었습니다.
		// delete k;

		// 삭제된 객체 k의 _hp를 건드리는 함수객체 위험합니다.
		job();

		// Functor(함수 객체) 로 표현하면

		class Functor
		{
		public:
			Functor(Knight* k) : _knight(k)
			{

			}

			void operator() ()
			{
				_knight->_hp = 200;
			};
		public:
			Knight* _knight;
		};

	}


	return 0;
}