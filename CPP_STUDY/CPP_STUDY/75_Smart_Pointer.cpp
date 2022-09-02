#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <algorithm>


// 오늘의 주제 : 스마트 포인터 ( smart pointer )


/*
	스마트 포인터 개론 입니다.

	이번 주제인 스마트 포인터는 제대로 사용하려면 멀티 쓰레드(?) 환경에서도 잘 돌아가도록 꼼꼼히 만들어야 되기 때문에
	나중에 서버시간에 배울 아토믹(?)문법이랑 등등을 활용해야 할겁니다. 지금은 러프하게 그냥 훑어보는 간략한 수업입니다.
	(잘 모르는 용어가 많이 나왔습니다.)

	스마트 포인터는 왜 필요한가?
	
	- C++의 장점이자 단점, 양날의 검인 포인터에 대해 얘기해야합니다. 포인터는 직접적으로 메모리를 읽거나 수정할 수 있는 장점이
	있었지만 위험한 메모리를 건드릴 수도 있다는 단점도 있었습니다.
	물론 위험한 만큼 조심해서 사용하면 되지 않을까? 라는 생각도 듭니다만. 아무리 조심해도 오랫동안 이어진 프로젝트에서 
	한번 실수 하게되면 바로 역적이 될수 있을만큼 치명적인 버그가 날 수 있습니다. 
	거기다 그 긴 기간동안 멤버가 하나도 안바뀔수는 없고 새로온 사람이 실수 하거나 잠시 안일하게 생각해서 메모리 오염이 
	날수 있는 리스크는 너무나도 큽니다.

	일반 포인터의 사고 예시 : 매번 등장하는 기사 클래스가 체력100 과 공격력10 그리고 타겟이라는 기사 포인터를 가지고 있습니다.
	타겟은 서로 공격하는걸 구현하려고 넣었습니다. 이후 기사1 과 기사2를 동적할당으로 생성하였고 기사1의 타겟으로 기사2 를 주었습니다.
	기사 클래스는 공격이라는 멤버 함수를 가지고 있어서 타겟의 체력을 자신의 공격력 만큼 차감하는 함수입니다. 
	기사1이 공격 함수를 실행한후 기사2의 체력은 90이 되었습니다. 
	여기까지는 문제가 없습니다. 
	이번에는 기사1의 타겟으로 기사2를 주는것까지는 동일하지만 기사1이 공격 함수를 실행하려는 사이 기사2가 로그아웃을 해서 
	기사2를 delete해주었습니다. 이후 기사1은 기사2가 로그아웃 한걸 모르고 공격 함수를 실행하면? 기사2 가 있던 메모리는 이미 
	싹 밀린 상태라서 엉뚱한 메모리를 수정하게됩니다.

	어떤 객체1이 다른 객체2의 포인터를 들고 있다면 객체1가 들고 있는 포인터가 nullptr로 안전하게 초기화 하기전 까지는 
	객체2를 delete하면 안되지만 C++의 포인터는 그런 안전장치가 없이 실수하면 객체1이 들고있는 포인터가 nullptr로 밀리기전에
	객체2를 delete할 수 있는 여지가 있다는 이야기 입니다.

	열심히 배운 쌩 포인터, 하지만 요즘 프로그래밍에서는 쌩 포인터를 그대로 사용하지 않고, 스마트 포인터로 간접적으로 사용합니다.
	우리 게임은 극한의 효율을 추구하니깐 쌩 포인터를 쓰겠어! 라고 하더라도 이미 언리얼 엔진에서 전부 스마트 포인터를 사용하고 있습니다.
	결국 어느정도 성능에 타협을 하더라도 안정성을 담보하고 가는게 좋겠습니다.

	--------------------------------------------------------

	* 그러면 스마트 포인터는 무엇인가?
	스마트 포인터 : 포인터를 알맞는 정책에 따라 관리하는 객체 ( 포인터를 래핑해서 사용 )
	
	스마트 포인터 표준에서 세가지 개념을 알아야 하는데
	shared_ptr, weak_ptr, unique_ptr

	1) shared_ptr : shared_ptr은 포인터를 관리를 하기는 하는데 reference count도 관리하기 시작합니다. 
		reference count는 말그대로 참조 카운트, 이 포인터를 누가 참조하고 있는지 그 수를 추적하고 이 포인터를 아무도 
		참조하지 않을때 그때에만 포인터의 메모리를 해제한다는 특징이 있습니다. 
		위의 쌩 포인터의 예시도 누군가 이 포인터를 참조하고 있는데 그냥 메모리를 해제했을때 일어나는 메모리 오염이었습니다.

		shared_ptr을 정식 방법은 아니지만 개념을 이해하기 위해 간략하게 구현해보겠습니다. shared_ptr을 sptr이라고 부르겠습니다.
		sptr은 우선 템플릿을 사용합니다. 포인터로 사용하기 때문에 어떤 타입이든 기능해야 하기 때문입니다. 
		멤버 변수로 템플릿 포인터를 하나 가지고 있고 기본은 nullptr로 초기화 해줍니다. 두번째 멤버 변수로 reference count를 가지고 있습니다.
		referenc count는 sptr내부가 아닌 밖으로 빼서 공용 메모리로 관리하는게 일반적이라고 합니다. 
		외부에 RefCountBlock이라는 클래스를 만들고 이 클래스는 _refCount = 1;이라는 int형 멤버 변수를 들고 있습니다.
		그러면 sptr의 두번째 멤버 변수로 RefCountBlock 포인터 _block = nullptr;로 가지고 있겠습니다.
		
		이제 생성자를 만들어야 하는데 기본생성자는 아무 내용없이 만들게 되면 이미 멤버 변수 둘다 nullptr로 초기화 해주었기때문에 
		nullptr을 가리키고 있는 포인터 처럼 객체가 생성될 것입니다.
		
		다음 생성자로 템플릿 포인터를 받는 생성자를 만듭니다. sptr을 생성할때 이 포인터를 가리켜라 할때 호출되는 생성자 입니다.
		_ptr을 인자로 받은 포인터로 초기화를 해줍니다. 그 다음 sptr은 reference count를 관리하는 객체이니깐 count를 하나 증가 시켜줘야합니다.
		현재 count를 담당하는 _block은 nullptr이기때문에 new로 동적할당을해주면 RefCountBlock의 기본 데이터가 1이니깐 딱 맞습니다.
		if 문으로 _ptr이 nullptr이 아니면 _block = new RefCountBlock(); 을 해줍니다.

		그 다음 생성자는 복사 생성자 입니다.  두 rptr이 하나의 포인터를 가리킬때 사용됩니다. 인자로 const 자신 클래스 참조 타입을 받습니다.
		_ptr, _block을 인자로 받은 sptr의 _ptr, _block으로 초기화 해주고 if 문으로 현재 _ptr이 nullptr이 아닌지 체크 후 아니라면 
		reference count를 ++ 시켜줍니다.

		이 다음은 복사 대입 연산자 입니다. 포인터는 생성하면서만 값을 받을수 있는게 아닌, 일단 nullptr로 생성후 필요할때 = 연산자로 데이터를 
		가리킬 수 있습니다. 기본 골자는 복사 생성자와 같습니다. 인자로 const rptr 참조타입을 받고 자신의 _ptr, _block에 값을 넣어줍니다. 
		그 후 if문으로 _ptr != nullptr 체크 _block의 count++을 해줍니다.

		마지막으로 소멸자를 만들어 줄겁니다. 이 소멸자가 중요한데 적절히 조건을 주어서 reference count가 있다면 원본 데이터를 날리지 않도록
		해야 합니다. 객체가 소멸되면서 우선 자신의 _ptr이 nullptr이 아니라면 ref count를 하나 줄여줍니다. 그리고 
		이중 if로 count가 0일때, count가 0이라면 아무도 원본 데이터를 참조하고 있지 않다는 상태이고 메모리 오염의 위험도 없어집니다.
		그럴 때 _ptr과 _block을 delete해줍니다. 
		즉 어떤 sptr이 사라질때 count를 하나 줄이는데 혼자 만 가리키던 포인터 였으면 바로 
		원본을 가리키던 _ptr을 delete해주고 필요없어진 _block도 delete 해줍니다.
		또는 sptr이 사라질때 이 sptr이 가리키던 데이터를 참조하는게 둘 이상이었다면 그냥 ref count만 1 줄이고 소멸됩니다. 
		마지막으로 참조하던 sptr이 사라질때 데이터들을 정리하게 됩니다.

		사용할때는 shared_ptr<타입> 변수이름; 이나 shared_ptr<타입> 변수이름 = new 클래스();, shared_ptr<타입> 변수이름(new 클래스());
		으로 사용합니다.
		지금 까지 배웠던 new delete문법은 이렇게 쓰고 실제로 shared_ptr을 쓸때는 
		shared_ptr<타입> 변수이름 = make_shared<타입>(); 로 사용합니다.
		이쪽이 메모리 블럭(?)을 이용해 좀 더 성능이 좋다고 합니다. (메모리 블럭이 무슨 말인지 잘 모르겠네요)
		
		
		shared_ptr의 문제점 

		메모리 오염을 막아주는 shared_ptr도 모든면에서 완벽한건 아닙니다. 
		사이클 문제가 있다고 하는데요 사이클 문제가 무엇이냐? 순환구조에서의 문제 입니다. 
		예를 들면 다시 기사 클래스로 기사1 과 기사2 를 shared_ptr로 동적할당해서 생성했습니다. 기사1과 기사2가 서로를 타겟으로 설정하였습니다.
		그러면 기사1을 참조하는 포인터가 둘이고 기사2를 참조하는 포인터가 둘입니다. 자기자신하나와 타겟으로 서로를 가리키고 있으니깐요
		여기서 어떤 문제가 생기냐면 만약 이대로 프로그램이 종료되어 두 기사들이 소멸한다고 했을때 자기자신이 소멸하면서 참조가 하나씩 줄고나면 
		아직도 원본 기사는 참조 카운트가 1씩 남아있습니다. 가리키던 기사 shared_ptr은 소멸 했는데도 말이죠 참조 카운트가 남아있으니 shared_ptr에 의해
		원본 객체들은 메모리를 해제하지 못하고 남아있을것 입니다. 실제로 서비스중인 게임에서 이런 실수가 일어나면 메모리 누수가 일어나서 몇일 후 
		메모리 부족으로 크래쉬가 날 것입니다. 해결방법은 프로그램 종료전에 각 기사들의 타겟을 nullptr로 초기화 해주면 되겠습니다.

		이런 shaed_ptr의 단점을 커버해주는 weak_ptr이 있습니다.


	2) weak_ptr
		
		shared_ptr의 단점을 보완하기 위한 weak_ptr입니다. 
		일단 기본적인 사용법은 순환 오류가 일어날것 같은 데이터에 shared_ptr 대신 weak_ptr을 사용합니다.
		그리고 shared_ptr에서 관리하던 RefCountBlock의 refCount외에 또 한가지weakCount를 관리하게 됩니다. refCount는 이 RefCountBlock 객체를 참조하는 
		애가 몇 명인지를 체크하는 것이고, weakCount는 몇개의 weak_ptr이 RefCountBlock를 참조 하고 있는지를 카운트 합니다.
		이 weakCount는 shared_ptr에서 순환 오류가 일어날 만한 데이터가 서로를 참조할때 refCount를 증가 시키지 않고 weakCount를 증가 시킵니다. 
		그래서 shared_ptr이 소멸될 때 refCount가 0이 아니어서 메모리누수가 일어나지 않게 해줍니다. 
		weak_ptr이 등장하면 shared_ptr의 소멸자에서 _block(RefCountBlock를 참조하고 있는 shred_ptr의 멤버 변수)의 삭제를 미룹니다.
		_block이 삭제되지 않아야 shared_ptr이 소멸된 후에도 weak_ptr이 자체적으로 lock()이라는 메소드로 자신이 속한 shared_ptr이 true인지 false인지를 
		체크해서 true이면 shared_ptr이 아직 소멸하지 않았으니 shared_ptr을 반환하고 false이면 shared_ptr이 소멸한 것이니 weak_ptr도 소멸하면서 
		_block을 같이 삭제 할것 같습니다. 이 부분은 제 예측 입니다. 

		weak_ptr은 마치 shared_ptr을 감싸고 있는것 같습니다. weak_ptr타입의 변수는 이전 처럼 그냥 쓸 수 없고 한단계씩 거쳐서 사용해야합니다. 
		우선 weak_ptr<Knight> _target; 이 위의 예시 중 타겟입니다. 공격 함수에서 이 타겟을 사용하는데 사용하기전 
		if (_target.expired() == false) 로 타겟이 만료 되었는지를 확인합니다. 만료되지 않았다면 _target.lock(); 을 사용하는데 이 메소드는 
		가리키던 대상을 shared_ptr로 반환해줍니다. 반환받은 shared_ptr은 원래 사용하던대로 사용하면 되겠습니다.


	3) unique_ptr 

		유니크 포인터는 간단히 어떤 주소를 가리키는 포인터이지만 자신만이 유일하게 그 주소를 가리키고 있도록 하는 포인터 입니다. 
		원래 사용했던 쌩 포인터와 거의 비슷한데 복사하는 부분만 막혀있다고 보면 되겠습니다.

*/


class Knight
{
public:
	Knight() { cout << "Knight 생성" << endl; }
	~Knight() { cout << "Knight 소멸" << endl; }

	void Attack()
	{
		// waek_ptr 타입인 _target을 사용할때는 expired로 이 포인터가 참조하고 있는 객체가 소멸되었는가 를 체크 후 
		// lock()이라는 메소드로 shared_ptr을 반환해 그 shared_ptr을 사용합니다.
		if (_target.expired() == false)
		{
			shared_ptr<Knight> sptr = _target.lock();

			sptr->_hp -= _damage;

			cout << "HP : " << sptr->_hp << endl;

		}
	}

public:
	int _hp = 100;
	int _damage = 10;
	weak_ptr<Knight> _target;

};

// shared_ptr 구현해보기

// refernce count는 shared_ptr밖에 빼서 공용 메모리로 관리하는게 일반적이라고 합니다.
class RefCountBlock
{
public:
	int _refCount = 1;
};

template<typename T>
class SharedPtr
{
public:
	// 아무것도 주지 않은 shared_ptr은 nullptr 처럼 작동합니다.
	SharedPtr() { }

	SharedPtr(T* ptr) : _ptr(ptr)
	{
		if (_ptr != nullptr)
		{
			_block = new RefCountBlock();
			cout << "RefCount : " << _block->_refCount << endl;
		}
	}

	// 복사 생성자
	SharedPtr(const SharedPtr& sptr) : _ptr(sptr->_ptr), _block(sptr->_block)
	{
		if (_ptr != nullptr)
		{
			_block->_refCount++;
			cout << "RefCount : " << _block->_refCount << endl;
		}
	}

	// 복사 대입 연산자
	void operator= (const SharedPtr& sptr)
	{
		_ptr = sptr._ptr;
		_block = sptr._block;

		if (_ptr != nullptr)
		{
			_block->_refCount++;
			cout << "RefCount : " << _block->_refCount << endl;
		}
	}

	~SharedPtr()
	{
		if (_ptr != nullptr)
		{
			_block->_refCount--; 
			cout << "RefCount : " << _block->_refCount << endl;
			
			if (_block->_refCount == 0)
			{
				delete _ptr;
				// weak_ptr이 등장하면 _block의 삭제를 미룬다.
				//delete _block;
				cout << "Delete Data" << endl;
			}
		}
	}

public:
	T* _ptr = nullptr;
	RefCountBlock* _block = nullptr;
};

int main()
{
	{
		//Knight* k1 = new Knight();
		//Knight* k2 = new Knight();

		//k1->_target = k2;

		//// k2라는 유저가 로그아웃
		//delete k2;

		//// k2 가 삭제되었다고 k1의 target이 nullptr로 바뀌지는 않습니다. 결국 메모리 오염이 일어납니다.
		//k1->Attack();
	}


	// shared_ptr 실습 

	// 생성후 소멸
	{
		cout << "shared_ptr 생성후 소멸" << endl;
		SharedPtr<Knight> k1(new Knight());


	}
	cout << endl;
	// 복사 생성자, 복사 대입 연산자 로 한 객체를 두개의 shared_ptr이 가리키고 있을 때
	SharedPtr<Knight> k2;
	{
		cout << "복사 생성자, 복사 대입 연산자 로 한 객체를 두개의 shared_ptr이 가리키고 있을 때" << endl;
		SharedPtr<Knight> k1(new Knight());
		k2 = k1;


	}
	cout << endl;
	
	// 일반 포인터의 문제점이었던 use after delete를 shared_ptr로 구현 해보겠습니다.
	{
		cout << "일반 포인터의 문제점이었던 use after delete를 shared_ptr로 구현 해보겠습니다." << endl;
		// make_shared는 이전 new 문법에 비해 메모리 블럭(?)을 사용해 더욱 성능좋게 동적 할당 해줍니다.
		shared_ptr<Knight> k1 = make_shared<Knight>();
		{
			// 이 유효범위를 지나면 k2가 소멸될 것이니 로그아웃한 효과
			shared_ptr<Knight> k2 = make_shared<Knight>();

			k1->_target = k2;
		}

		k1->Attack();
		

	}
	cout << endl;


	// shared_ptr의 문제점 사이클
	{
		cout << "shared_ptr의 문제점 사이클" << endl;

		shared_ptr<Knight> k1 = make_shared<Knight>();
		
		shared_ptr<Knight> k2 = make_shared<Knight>();

		// 서로를 타겟으로 가리키게 되면 프로그램 종료시 메모리 누수가 일어납니다.
		k1->_target = k2;
		k2->_target = k1;

		// 해결 손수 타겟을 nullptr로 밀어줍니다.
		/*k1->_target = nullptr;
		k2->_target = nullptr;*/
		
	}
	cout << endl;
	return 0;
}