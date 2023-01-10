#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>



// 오늘의 주제 : enum class

/*
	C++11에 새로 추가된 문법인 enum class (scoped enum)
	이전에 배운 enum은 열거형(enumerated type, enumeration)이라는 자료형입니다.

	enum과 enum class는 각각 장단점이 있어서 상황에 따라 쓴다고 합니다. 각각의 장점에 대해 알아보겠습니다.

	enum class
	1) 이름공간 관리 (scoped)
	- 이름들의 유효범위가 중괄호 안으로 한정되어 다른 곳에서 다시 이름이 쓰여도 괜찮게 되었습니다.

	2) 암묵적 변환 금지
	- enum class의 데이터들은 enum과 다르게 암북적 변환이 막혀있고 굳이 변환해서 사용하려면 명시적으로 캐스팅을 해줘야 합니다.
	이런 특징이 무조건 enum보다 enum class가 더 좋다는 뜻이 아닙니다. 오히려 더 귀찮아 질수 있습니다.
	enum class는 그 데이터들이 int같은 걸로 인식되는것이 아닌 그냥 enum class 타입으로 인식되기 때문에 예전 Text RPG때 input값과 
	비교하거나 하는 것들을 하기위해서는 일일히 캐스팅을 해줘야합니다.

	enum (unscoped enum(범위없는))
	1) 이름공간 관리
	- 기존 enum은 중괄호안에 열거할 데이터들을 담아놓긴하지만 유효범위가 중괄호 안에만 한정된것이 아닌 사실상 전역적으로 사용됩니다.
	  이전 실습에서 열거형 Knight앞에 PT_Knight처럼 PT를 붙여준이유가 enum에서 이름을 써버리면 다른 곳에서 재사용이 불가능해집니다.

	2) 암묵적 변환금지
	- enum의 데이터들은 사실 enum타입의 숫자들과 똑같은데 편의상 이름을 지어준것이기 때문에 다른 숫자타입에 대입하게되면 암묵적으로 변환이 됩니다.


	결론 : enum이던 enum class든 각각 장단점이 있습니다. 8 대 2로 enum을 더 많이 쓰고 이름을 신경써서 지어주는걸 선택한다고 합니다. 하지만 
		enum class도 사용하는 분들도 계시니깐 어떻게 사용하는지 알고는 있어야 겠습니다.
	*/

// unscoped enum  (범위없는)
enum PlayerType : char  // 기본은 int형이지만 이렇게 타입을 지정해줄수 있습니다.
{
	PT_Knight,
	PT_Archer,
	PT_Mage,
};

// scoped enum
enum class ObjectType
{
	Player,
	Monster,
	Projectile,
};

int main()
{
	int input;
	cin >> input;

	// enum	- int와 비교가 간편
	if (input == PT_Knight)
	{

	}
	
	// enum class - int와 비교하려면 캐스팅 필요
	if (input == static_cast<int>(ObjectType::Player));
	{

	}

	return 0;
}