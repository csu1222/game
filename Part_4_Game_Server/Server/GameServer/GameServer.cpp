#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"

/*
shared_ptr 의 순환 문제 의사코드
*/

using KnightRef = TSharedPtr<class Knight>;
using InventoryRef = TSharedPtr<class Inventory>;

class Knight : public RefCountable
{
public:

	Knight() 
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	void SetTarget(KnightRef target)
	{
		_target = target;
	}

	void SetInventory(InventoryRef inven)
	{
		_inventory = inven;
	}

	KnightRef _target = nullptr;
	InventoryRef _inventory = nullptr;
};

class Inventory : public RefCountable
{
public:
	Inventory(KnightRef knight) : _knight(**knight)
	{

	}

	Knight& _knight;
};


int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가 
	// 2) 순환(Cycle) 문제 

	KnightRef k1(new Knight());
	k1->ReleaseRefCount();
	//KnightRef k2 = new Knight();
	//k2->ReleaseRefCount();

	k1->_inventory = new Inventory(k1);

	//k1->SetTarget(k2);
	//k2->SetTarget(k1);

	k1 = nullptr;
	//k2 = nullptr;

	// 표준 shared_ptr 버전 
	// shared_ptr의 코드를 따라 들어가보면 _Ptr_base 라는 클래스를 상속받고 있고 
	// 이 _Ptr_base 클래스는 shared_ptr 과 weak_ptr 을 위한 베이스 클래스라고 주석이 적혀있다
	// _Ptr_base의 멤버 변수는 element_type* 타입 하나와 _Ptr_count_base* 타입의 변수가 있다
	// 또 shared_ptr을 만들때는 생성자를 사용하는 것보다 make_shared를 사용하는 편이 좋다고 하는데
	// 그 이유는 make_shared 코드를 살펴보면 Knight로 넘겨준 것이 템플릿으로 new _Ref_count_obj2
	// 하고 있습니다. 또 이 클래스 코드를 따라 가보면 절묘하게도 아까 살펴본 _Ref_count_base를 
	// 상속하고 있습니다. 
	// 이것은 우리가 만든 RefCountable 클래스를 상속받는것처럼 된다는 겁니다. 
	// [Knight* | RefCountingBlock] 
	// make_shared로 만들게 되면 위의 [] 와 같이 Knight 객체와 더해서 레퍼런스 카운팅을 하는 객체를 
	// 한번에 공간을 할당해 만들어 줍니다. 
	// 그러면 이번에는 RefCountingBlock에 해당하는 객체는 어떤 데이터를 가지고 있을까요
	// 직접 만들었던 RefCountable 을 생각해보면 레퍼런스 카운트를하는 정수를 가지고 있을것 같은데 
	// 또 타고 들어가면 Uses 와 Weaks 라는 두가지 아토믹 카운터가 있습니다. 



	shared_ptr<Knight> spr = make_shared<Knight>();
	weak_ptr<Knight> wpr = spr;

	// weak_ptr을 사용하려면 expired() 를 체크해야한다

	bool expired = wpr.expired();
	// wpr이 가리키는 객체가 아직 존재 하는건지를 bool값으로 체크하는것입니다. 

	// 만약 매번 expired를 체크하기 번거롭다면 wpr.lock()을 한후 다시 shared_ptr로 캐스팅해 사용합니다. 
	shared_ptr<Knight> spr2 = wpr.lock();


}