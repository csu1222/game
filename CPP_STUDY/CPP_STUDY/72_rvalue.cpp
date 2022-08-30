#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <algorithm>


// 오늘의 주제 : 오른값(rvalue) 참조와 std::move

/*
	
	오늘 배우는 rvalue reference 같은 경우 사실 일반적으로 컨텐츠를 만들때는 생각보다 자주 활용할 일은 없을거라고 합니다.
	하지만 코어 라이브러리(프로젝트에서 빠질수 없는 중요 라이브러리)를 만들때는 종종 사용할때가 있을 것이라고 합니다.

	이 rvalue reference를 자주 사용하느냐 아니냐를 따지기 전에 모던 c++에서 꼭 배워야 하는 이유는
	이것이 C++11 에서의 가장 주요한 변경점중 하나이기 때문입니다. 
	이 rvalue reference때문에 C++11 이전 코드와 이후의 코드가 속도 차이가 나는 이유입니다.
	어쩌면 오른값 참조가 C++11의 꽃이라고 볼 수도 있습니다. 그래서 활용을 자주 안하더라도 내용 자체는 꼭 숙지해야 겠습니다.


	우선 오른값이란 용어가 나왔습니다. 그러면 왼값도 있을것 같습니다. 두 용어? 개념을 비교해보겠습니다.

	왼값(lvalue)  vs   오른값(rvalue)
	- lvalue : 단일식을 넘어서 계속 지속되는 개채
	- rvalue : lvalue가 아닌 나머지 (임시 값, 열거형, 람다, i++ 등)

	설명을 봐도 잘 모르겠습니다. 추가로 설명을 해보면
	int a = 3; 에서 a 라는 개체는 등호를 기준으로 왼쪽에 있습니다.(그렇다고 오른쪽에 등장하면 모두 rvalue라는 이야기는 아닙니다.)

	3 = a; 
	에서는 에러메세지로 3은 lvalue여야 하다고 나옵니다.
	위에서 rvalue의 예시인 a++ 을 다시 왼쪽 항에 두면
	(a++) = 3; 
	이것도 식이 수정할 수 있는 lvalue가 와야 한다고 그럽니다.
	
	그리고 왼값은 계속 지속된다고도 했습니다. 이말은 a라는 변수를 다시 4라는 값을 넣거나 *연산자같은거로 연산을 한다거나
	하면서 a를 계속 사용할수 있었습니다.

	3이라는 상수를 임시값으로 사용해서 int a를 생성했었습니다. 
	rvalue의 예시로 드는 임시값입니다.
	rvalue들은 왼값으로 동작하지 못하지만
	lvalue로 오는 값들은 오른값으로 동작하기도 합니다.(ex) int b = a; )


	이번에는 int형 보다 조금 더 복잡한 class를 다시 예로 들어보겠습니다.

	복습으로 Knight라는 클래스가 많은 용량의 멤버 변수를 가지고 있다고 가정해보겠습니다.
	어떤 함수에서 이 Knight클래스를 수정한다고 했을때 Knight를 인자로 받아야 하는데 이때 몇가지 방법이 있었습니다.
	1) 복사 방식으로 인자 받기: 매개변수를 (Knight k){ } 이런식으로 복사방식으로 받는 방법이 있습니다. 이 방법은 
		Knight의 많은 용량의 멤버변수 전체를 복사해야 하고 이 매개변수의 값을 조작한다고 해도 인자로 받은 원본은 
		건드릴 수 없었습니다. 복사 방식이 필요할 경우도 있겠지만 좀 찝찝한 방법이었습니다.
	2) 참조 방식으로 인자 받기: 매개변수를 (Knight& k){ } 이런식으로 참조형으로 받는 방법입니다. 이 방식은
		Knight클래스의 데이터 전체를 복사해오는것이 아닌 인자로 받은 원본의 주소만 가져오고 그 주소에 접근하는 방식
		입니다. 그래서 원본도 조작할 수 있습니다.(포인터 방식은 사실상 참조방식과 같으니 생략하겠습니다.)

	2번의 참조방식이 사실 lvalue reference 방식이었습니다. 

	클래스를 생성할때 생성자를 지정해주지 않으면 기본 생성자로 생성되었습니다. 
	ex) Knight k1 = Knight();
	이때 Knight()는 k1을 생성해주고 사라지는 임시값 이었죠, 위에서 말했듯 단일식을 넘어서 계속 지속되는 개채 말고 
	는 오른값이라고 했었습니다. 즉, Knight()는 오른 값입니다.

	void TestKnight_LValueRef (Knight& knight) { }
	문법 공부용으로 만든 함수가 있습니다. 인자로 Knight의 참조를 받는데 기본생성자로 객체를 생성하면서 주면 어떨까요?
	TestKnight_LValueRef(Knight());
	비 const참조 값에 대한 초기 값은 lvalue여야 합니다. 라는 에러메세지가 뜹니다. 다시 위에서 말했던 Knight()가 오른값이기 때문인가봅니다.
	Knight()는 Knight객체를 생성할때 딱 한번 사용하고 사라지는 임시개체입니다.

	매개변수를 const로 받아준다면 또 기본생성자 Knight()가 인자로 들어갈수 있게됩니다.
	void TestKnight_ConstLValueRef (const Knight& knight) { }
	에러메세지가 말한대로 const참조 값이면 rvalue가 들어갈수 있게되었습니다.

	왜 const없이는 오른값을 받아주지 않는걸까 생각해보면 이유를 알수 있습니다. 
	Knight()라는 생성자로 만든 객체는 어떤 변수에 담겨 유지되는게 아닌 함수가 끝나면 바로 사라질 데이터입니다. 
	const가 없으면 이런 오른값을 함수내에서 이리저리 수정해가면서 사용할수 있을 것입니다. 그런 헛일을 하는걸 컴파일러가 
	알아서 막아주는 것입니다.
	반면 const가 있다면 임시값을 직접 수정하지 않는다는걸 보장하고 그저 그 값들을 그대로 이용할 것이라는 걸 알수 있으니 
	const 참조값이면 컴파일러가 오른값을 받는걸 허용해주는 것입니다.

	const 참조를 받는 함수의 부작용? 단점?이라면 Knight클래스의 멤버함수 PrintInfo라는 함수가 있다고 했을때 
	TestKnight_LValueRef 의 구현부에 매개변수로 받은 객체의 PrintInfo멤버 함수를 호출하려면 그 PrintInfo함수도 
	const{ } 로 읽기 전용함수여야 호출 할수 있게됩니다.
	이유는 TestKnight_LValueRef의 매개변수가 읽기전용인데 그 매개변수의 매소드에서 값을 바꿀수 있는 여지가 있으면 말이 되지 않기 때문입니다.



	오늘의 주제인 오른값 참조는 C++11에서 나온 위 세가지 방법과 다른 문법입니다.
	우선 위의 예시로 오른값 참조문법을 적용하면
	TestKnight_RValueRef(Knight&& knight) { }
	형태만 보면 참조의 참조 같이 보입니다. 이전에 배운 다중 포인터도 연상이되지만 
	&& 이 문법은 그냥 오른값 참조라는 뜻만을 지닌 문법입니다.

	일반 왼값 참조 방식과 오른값 참조 방식을 어셈블리 수준에서 살펴보면 둘다 결국 스택메모리에 저장되어 있다가 
	그 주소가 push되면서 함수를 호출합니다. 저장 되어 있는 주소가 왼값의 경우 main함수의 스택위에 있었고 
	오른값의 경우 TestKnight_RValueRef함수의 스택위에 바로 저장된 객체 였습니다.

	오른값 참조를 사용하면 결국 금방사라질 임시값을 조작하는게 무슨 의미가 있는지 잘 모르겠습니다.
	오른값 참조 함수에 인자로 왼값인 k1이라는 객체를 캐스팅해서 주면 정상적으로 왼값이면서 오른값 참조방식으로 
	원본을 수정도 하고 휘발 되지도 않습니다.
	이러면 다시 돌고 돌아서 어셈블리 단계에서는 왼값 참조방식과 오른값 참조방식이 비슷하다고 했는데 굳이 오른값 참조라는 
	새로운 방식을 배우고 다시 인자를 캐스팅까지해서 줘야하는가? 라는 의문이 생깁니다.

	어셈블리 단계로 보면 헛수고를 하는것 같지만, C++ 단계에서 보면 의미가 생긴다고 합니다.
	C++ 단계에서 
	왼값 참조 : 내가 매개변수 원본을 넘겨줄테니 수정할수도 있고 넘겨줄수도 있고 니가 멋대로 가지고 놀면 돼!
	const 왼값 참조 : 내가 원본을 주지만 수정은 하지말아! 읽는건 허락해 줄게!
	오른값 참조 : 원본을 줄텐데 그걸 읽고 쓰고 하는것도 니 마음대로 할 수 있고!, 심지어 이 함수가 종료되면 
		걔는 더이상 활용하지도 않을거니까 니가 멋대로 해도 돼!(매개변수가 '이동대상'이 되었다는 힌트)

	'이동' 이라는 단어가 등장했습니다. 이단어는 나중에 다시 다루고 

	오른값 참조의 매개변수는 원본을 유지 하지 않아도 됩니다. 이 원본을 유지하지 않아도 된다는 것은 어떤 장점이 있을까요?
	
	오른값 참조의 장점을 알기 위해서 우선 예전에 배운 얕은 복사, 깊은 복사에 대해 복습해야합니다. 
	
	선제 조건으로 Pet이라는 클래스를 멤버 포인터 변수로 가지고 있는 Knight클래스가 있습니다. 
	Knight를 복사하는 연산자를 만들때
	얕은 복사 : 인자로 받은 객체의 멤버 변수를 하나하나 복사한다.
	- 이러면 인자의 pet을 가리키는 포인터를 복사한것이기 때문에 하나의 Pet객체를 두 Knight객체가 가리키게 됩니다.
	깊은 복사 : 인자로 받은 객체중 포인터가 아닌 멤버 변수는 각각 복사후 포인터 변수는 new Pet(매개변수.pet);로 
	같은 값을 가진 pet을 새로 생성해 할당한다.
	- 각각의 knight객체가 같은 값을가진 멤버변수들을 가지게 됩니다.

	얕은 복사는 보다 빠르지만 약간의 하자가 있고 깊은 복사는 보다 느리지만 의도대로 기능합니다.

	이때 오늘의 주제인 오른값 참조를 사용하면 두 복사방법의 절충점이 됩니다.
	왼값 참조는 원본을 수정, 읽기까지는 가능했지만 손상을 입힐수는 없었습니다.
	오른값 참조는 원본이 어짜피 사라질 데이터이기 때문에 수정, 읽기, 손상까지도 가능했습니다. 이 특징을 이용한 

	이동 생성자, 이동 대입 연산자입니다.
	오른값 참조로 받은 매개변수를 얕은 복사로 복사한후 매개변수를 지워버리는 것입니다.이러면 같은 값을 가진 데이터가 
	복사 되었고 포인터변수인경우 원본이 사라졌기 때문에 하나의 객체가 하나의 펫을 가리키게 되었습니다.
	매개변수에서 현재 객체로 값이 '이동'한 것과 같기 때문에 이동이라는 단어를 사용합니다.

	static_cast<Knight&&>(객체);  이렇게 캐스팅해서 사용하지만 더 간단히
	std::move(객체);	이 문법이 위의 캐스팅 문법과 동일한 기능을합니다.

	이 오른값 참조는 원본데이터가 손상되도 괜찮은 경우에만 사용합니다. 
	이런 경우가 실제 콘텐츠 제작에서는 별로 없다고합니다. 하지만 코어 라이브러리를 만들때는 빈번하다고도 합니다.
	오른값 참조가 성능상으로 좋기 때문에 쓰는 경우도 있지만 오른값 참조의 특징(원본이 없어져도 된다)가 유용해서 
	사용되는 경우도 있습니다.
	예시로 아직은 배우지 않은 유니크포인터가 있습니다. 이 포인터는 단 하나만 존재할수 있는 포인터로 복사를 하려고 하면
	에러가 납니다. 
	이 유니크 포인터를 그래도 다른 유니크 포인터에 값을 넘기려면 오른값 참조로 넘여야합니다. 값을 이동하고 나서 바로 원본이
	사라지기 때문에 유니크 포인터의 특징을 유지하면서 값을 이동할수 있습니다.

*/

class Pet
{
public:

};

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}
	// 복사 생성자
	Knight(const Knight& knight)
	{
		cout << "const Knight&" << endl;
	}
	// 이동 생성자
	Knight(Knight&& knight) noexcept
	{
		cout << "Knight(Knight&&)" << endl;
		_hp = knight._hp;
		_pet = knight._pet;

		knight._pet = nullptr;
	}
	~Knight()
	{
		if (_pet)
			delete _pet;
	}
	// 복사 대입 연산자
	void operator= (const Knight& knight)
	{
		cout << "operator= (const Knight&)" << endl;

		// 깊은 복사
		_hp = knight._hp;

		if (knight._pet)
			_pet = new Pet(*knight._pet);
	}
	// 이동 대입 연산자
	void operator= (Knight&& knight) noexcept
	{
		cout << "operator= (Knight&& knight)" << endl;

		// 얕은복사
		_hp = knight._hp;
		_pet = knight._pet;

		knight._pet = nullptr;

	}
	// const참조를 받는 함수에 쓰이려면 똑같이 const로 만들어 줘야합니다.
	void PrintInfo() const
	{

	}
public:
	int _hp = 100;
	Pet* _pet = nullptr;
};

void TestKnight_Copy (Knight knight) { }
void TestKnight_LValueRef (Knight& knight) { }
void TestKnight_ConstLValueRef(const Knight& knight) { knight.PrintInfo(); }
void TestKnight_RValueRef(Knight&& knight) { knight._hp = 200; }

int main()
{
	//int a = 3;
	//3 = a;	// 에러 메세지 : 식이 수정할 수 있는 lvalue여야 합니다. 

	Knight k1;

	TestKnight_Copy(k1);
	TestKnight_LValueRef(k1);
	// TestKnight_LValueRef(Knight());
	TestKnight_ConstLValueRef(Knight());
	TestKnight_RValueRef(Knight());
	TestKnight_RValueRef(static_cast<Knight&&>(k1));


	// 오른값 참조 예시
	Knight k2;
	k2._hp = 1000;
	k2._pet = new Pet();

	Knight k3;
	//k3 = static_cast<Knight&&>(k2);	// 이동 대입 연산자

	// 위의 static_cast와 동일한 의미
	k3 = std::move(k2);

	return 0;
}