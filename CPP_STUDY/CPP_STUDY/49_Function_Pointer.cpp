#include <iostream>
using namespace std;

// 오늘의 주제 : 함수 포인터


/*
지금까지 여러번 배운 포인터에 대해 복습하자면 
포인터 = 주소를 담는 바구니
포인터의 구성요소  1) 포인터		*
				   2) 변수이름		pointer
				   3) 데이터타입	int
그리고 포인터 자체는 주소를 담는 바구니 이다 보니 데이터 타입이 무엇이든 가리킬수 있습니다. 
여태껏 int나 우리가 만든 클래스를 가리키고는 했습니다.
그리고 typedef (무엇) (무엇) 으로 타입에 다른 이름을 붙여주기도 했습니다.
typedef int DATA;
라고 하면 int 대신 DATA를 쓸수도 있습니다.

포인터로 함수도 가리킬수 있습니다.
다른 데이터 타입은 이름들이 있어서 포인터로 가리키기 쉬웠는데 함수는 어떻게 타입을 지정할까요?
함수는 반환타입이라든지, 함수이름, 인자의 타입이나 갯수가 각기 다르기 때문에 이 시그니쳐가 중요합니다.

밑의 Add 함수를 예로 들면 
int Add(int a, int b) 가 Add 함수의 시그니쳐인데 이 중 함수이름은 우리가 직접 지은 것이기에 날려줍니다
int (int a, int b) 가 이제 이 함수에서 중요한 부분만 남은 것이라고 할 수 있습니다. 그럼 이 시그니쳐를 
타입으로 쓰기위해 typedef를 써보겠습니다.

typedef int (int a, int b) FUNC_TYPE;
이라고 하면 이 Add함수를 FUNC_TYPE으로 부르겠다는것 같지만 아닙니다. 함수를 typedef하려면 조금 다른데

typedef int(FUNC_TYPE)(int a, int b); 로 해야 typedef가 됩니다. 이런 불규칙한 문법변화는 했갈리지만 
처음 만들때부터 이렇게 만들어져서 어쩔수 없이 외워야 되겠습니다.

추가로 아직 배우지 않은 모던 C++에서는 using을 이용할 수도 있습니다.
using FUNC_TYPE = int(int a, int b);

그러면 이 FUNC_TYPE으로 포인터를 만들어보겠습니다.
FUNC_TYPE* fn;
1)포인터 :		*
2)변수이름 :	fn
3)데이터 타입:	함수(int(int a, int b))
이 되겠습니다.

함수 포인터 말고 이전에 쓰던 방식으로 Add를 호출하는걸 어셈블리 레벨에서 보면 인자 둘을 push해주고 
Add라는 주소로 call해주었습니다. (보기 좋게 Add지 사실은 실제 메모리 주소)이후 call한 주소를 따라가면 다시 
실제 Add함수가 있는 주소로 다시 call해주었습니다.
즉, 함수 이름은 함수의 시작 주소를 들고 있습니다. (배열과 유사)
그러다 보니 새로 만든 함수 포인터는 함수가 있을 주소를 담을수 있는데 Add자체가 함수의 시작주소를 들고 있으니 

fn = Add; 이렇게 값을 줄수 있습니다.

앞으로 int두개를 받아주고 int하나를 반환하는 함수는 fn에 담을수 있게되었습니다.

그냥 Add함수를 쓰면되지 왜 굳이 함수포인터 한단계를 거쳐서 써야하는가?
- 간단한 예시로 fn에 Add를 할당해서 쓰다가 Add를 쓰는자리에 똑같은 매개변수 수 , 똑같은반환 타입을 가진 뺄셈 함수로
	대체할때 Add를 직접썼으면 일일히 손으로 바꿔줘야하고 fn에 담아서 썻으면 fn에 할당한 Add만 Sub로 바꿔주면 되겠습니다.


그래도 잘 와닫지가 않네요 다른 예제를 들어보겠습니다.
Item이라는 클래스를 만들었습니다. 이 클래스는 멤버 변수로 아이템 아이디, 희귀도, 소유자 아이디를 가지고 있습니다. 
그리고 FindItem 이라는 함수를 만들었습니다. 반환은 Item* 아이템클래스의 포인터를 반환합니다. 받는 인자는 첫째로 아이템의 
배열을 받고 두번째로 그 배열의 크기를 받습니다. for문을 돌며 임시 변수에 각 아이템을 담고나서 이후 필요한 아이템을 찾게됩니다.

이 함수를 기본으로 무엇을 기준으로 아이템을 찾는지에 따라 여러 함수가 만들어 질것입니다. 비슷한 함수가 만들어질 때마다 이
FindItem을 복붙해서 쓴다면 너무 코드의 양이 많아지고 반복되는 부분이 많아집니다. 그리고 이후에 어떤 안전체크할 것이 늘어서 
같은 FindItem류의 함수들을 수정해야 한다면 함수 하나하나 찾아서 고쳐야합니다.

지금까지 함수의 인자로는 일반적인 타입의 변수들, 포인터, 참조값, 객체등을 넘겨주었는데 오늘 배운 함수 포인터로 '동작' 자체를
인자로 넘겨준다면 한 함수에서 필요한 동작을 골라 쓸수 있게 되는 것입니다.

typedef 로 함수 포인터를 넘겨 줄 수도 있지만 인자로 직접 넘겨줄 수도 있습니다.
typedef [반환타입](식별자)(인자);  이후 
func(*식별자); 이렇게 포인터로 넘겨주면 됩니다.

혹은 
func([반환타입](*식별자)(인자)) 
이렇게 직접 인자로 넣어줄 수도 있습니다.

실제 사용에서는 함수 포인터와 같은 시그니쳐인 함수를 구현 해두고 용도에 맞게 사용하면 되겠습니다.
ex) FindItem(item, 10, IsRareItem);

단점으로는 같은 시그니처를 가진 함수만 사용할 수 있습니다. 
예시로 bool IsOwnerItem(Itme* item, ownerId); 라는 소유자 아이디를 추가로 넘겨주어서 해당하는 아이템을 찾는 함수라면
인자 갯수가 맞지 않아서 함수포인터로 사용 할수 없습니다.

이 경우를 지금 껏 배운 내용으로 구현하고자 한다면 함수 포인터를 인자가 가장 많은 경우로 만들어주어서 필요없는 함수의 경우
아무 값이나 넣어서 넘기고 두번째 인자가 필요한 함수일때 사용하는 걸로 구현할수 있을 것입니다.
*/

int Add(int a, int b)
{
	return a + b;
}

int Sub(int a, int b)
{
	return a - b;
}

// 또 다른 예제
class Item
{
public:
	Item() : _itemId(0), _rarity(0), _ownerId(0)
	{

	}
public:
	int _itemId;	// 아이템을 구분하기 위한 ID
	int _rarity;	// 희귀도
	int _ownerId;	// 소지자 ID
};

typedef bool (ITEM_SELECTOR)(Item* item);
// 또는

// 인자로 바로 함수포인터를 넘겨주었습니다.
Item* FindItem(Item items[], int itemCount, bool (*selector)(Item* item, int find), int value)
{
	for (int i = 0; i < itemCount; i++)
	{
		Item* item = &items[i];
		// TODO 조건
		if (selector(item, value))
			return item;
	}
	return nullptr;
}

// 레어 아이템을 찾는 함수
bool IsRareItem(Item* item, int rarity)
{
	return item->_rarity >= rarity;
}

bool IsOwnerItem(Item* item, int ownerId)
{
	return item->_ownerId == ownerId;
}
int main()
{
	int a = 10;
	int* pointer = &a;

	typedef int(FUNC_TYPE)(int a, int b);

	// 함수 포인터 생성
	FUNC_TYPE* fn;

	fn = Add;

	// 이전방식 함수 호출
	int result = Add(1, 2);

	// 함수 포인터로 함수 호출
	result = fn(1, 2);  // 함수포인터 기본 문법

	result = (*fn)(1, 2); // 함수 포인터는 *(접근 연산자)를 붙어도 함수주소

	
	// 함수포인터의 활용 : 인자로 함수를 넣어주기
	Item item[10] = {};
	item[3]._rarity = 2;  // RARE
	Item* rareItem = FindItem(item, 10, IsRareItem, 2);


	return 0;
}