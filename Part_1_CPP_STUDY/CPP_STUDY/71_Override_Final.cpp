#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <algorithm>


// 오늘의 주제 : override, final

/*
	override, final은 가상함수와 관련된 키워드들입니다. 가상함수가 객체지향의 꽃이라고도 합니다.

	override는 재정의라는 뜻으로 예를 들면 Player라는 클래스를 상속받은 Knight클래스가 있고  
	부모 클래스인 Player에서 Attack이라는 함수가 있다고 했을때 Knight에서도 Attack함수를 자기 나름대로 고쳐쓰는게 
	override(재정의)입니다.

	비슷한 키워드로 over loading (함수 이름의 재사용)이 있습니다. 같은 이름의 함수인데 매개변수의 타입이나 갯수가 다를때 
	같은이름으로도 사용할수 있는것입니다.

	복습) 부모클래스타입의 변수를 자식 클래스로 생성할 수 있었습니다. 그럴때 오버라이드한 함수를 실행하면 부모 클래스의 함수를
	불러옵니다. 원본 클래스의 함수를 불러오려면 부모 클래스의 함수에 virtual을 붙여주면 가상함수테이블에서 원본 클래스를 추적해서 
	실행해 주었습니다.

	이번에 배울 override는 복습의 오버라이드말고 문법적인 override입니다. 
	용도는 오버라이드된 함수 매개변수 뒤에 override라고 붙여주면 이 함수가 부모 클래스에게서 오버라이드된 함수라는걸 확인시켜줍니다.
	꼭 이게 필요한것인지 예시를 들어보면 알 수 있습니다. 

	예시) 가상함수(virtual)로 만든 오버라이드 함수는 앞에 virtual을 부모클래스에게만 붙힐수 있는게 아닌 자식 클래스의 함수에도 
	붙힐수 있습니다. 
	그러면 이 함수가 가장 원본?인 클래스인지 했갈립니다. 이게 왜 문제가 되냐면 클래스의 상속 관계중에 중간에 생긴 가상함수부터
	오버라이드 된 함수는 더 상위 클래스에서 호출하면 에러가 뜨게됩니다.
	또 다른 문제는 어쩌다가 자식클래스에서 오버라이드된 함수의 시그니쳐를 변경하게되면 이 함수는 더이상 오버라이드된 함수가 아닌 
	개별 함수가 되어버립니다.(const를 붙히거나하는경우) 이럴때는 빌드도 통과되지만 자식클래스를 부모 클래스로 캐스팅했을경우
	부모클래스의 함수가 호출이 될것입니다. 더이상 가상함수테이블로 추적이 되지않는 별개 함수이기 때문입니다.

	예시와 같은 상황에서 override 문법을 사용하게되면?
	우선 override를 붙혀도 정상적으로 통과가 된다면 이 함수는 상위의(부모의) 클래스의 함수가 오버라이드된 함수입니다. 
	굳이 상속을 따라 올라가면서 확인하지않아도 되서 가독성이 좋아 졌습니다. 만약 문법에러가 재정의(override)하지 않았다고 
	뜬다면 지금 이 함수가 가상함수중 최상위의 함수인 것입니다.
	두번째로 자식클래스의 함수의 시그니쳐가 달라졌을때도 override문법을 붙여보면 바로 빌드에서 에러로 재정의 되지 않았다고 뜹니다. 
	이유는 아예 이 함수는 부모의 가상함수와 다른 함수 즉, 최상위 함수가 되어서 그렇습니다.

	final
	그러면 final은 무엇이냐!
	final은 잘사용되지는 않지만 알아둬야합니다. override와 어느정도 비슷한 역할을 하지만 약간 다릅니다.
	의미는 이 함수를 마지막으로 더 하위로 오버라이딩하지 않게 막습니다. 그리고 override의 의미도 겸합니다.
*/

class Creature
{
public:
	virtual void Attack() 
	{
		cout << "Creature!" << endl;
	}

};

class Player : public Creature
{
public:
	virtual void Attack() override
	{
		cout << "Player!" << endl;
	}
};

class Knight : public Player
{
public:
	// 재정의 (override)
	virtual void Attack() final
	{
		cout << "Knight!" << endl;
	}
	
	// 오버로딩(overloading) : 함수 이름의 재사용
	int Attack(int n)
	{
		cout << "Attack(int)" << endl;
		return n;
	}
};

int main()
{
	// Knight 객체에서 Attack함수를 직접부르면 정상 작동
	Knight* k1 = new Knight;

	k1->Attack();

	cout << "-------------" << endl;

	// Player클래스가 부모 클래스이니 자식 클래스인 Knight로 캐스팅이 되었습니다.
	Player* p1 = new Knight;

	p1->Attack();
	

	cout << "-------------" << endl;

	Creature* c = new Knight;

	// Creature에는 Attack함수가 멤버가 아닙니다.
	// c->Attack();



	return 0;
}