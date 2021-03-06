#include <iostream>
using namespace std;

// 오늘의 주제 : 포인터 연산

// 1) 주소 연산자 (&)
// 2) 산술 연산자
// 3) 간접 연산자
// 4) 간접 멤버 연산자

struct Player // 간접 멤버 연산자 예시
{
	int hp;
	int damage;
};

int main()
{
	int number = 1;

	// 1) 주소 연산자 (&)
		// - 해당 변수의 주소를 알려주세요 -
		// - 더 정확히 말하면 해당 변수 타입(TYPE)에 따라서 TYPE* 반환
		int* pointer = &number;

	// 2) 산술 연산자 (+ -)

		// 변수에 대해 산술연산자를 이 이상 다양한 방법으로 사용 했었습니다.
		number = number + 1;
		number++;
		++number;
		number += 1;

		// 포인터에도 똑같이 사용할 수 있습니다.
		pointer = pointer + 1;
		pointer++;
		++pointer;
		pointer += 1;

		// 하지만 포인터 산술연산자 에는 특이한 점이 있습니다.
		/*
		예를 들어 
		number = number + 1;

		pointer = pointer + 1;

		이 둘을 비교할때 위의 int 변수 number는 일반적으로 생각하듯 정수 1이 증가 했습니다.
		반면, 포인터 pointer를 메모리에서 까보면 +1 을 하니 number를 가리키는 주소값에서 
		4가 증가했습니다(?)

		그 이유는 포인터를 선언 할 때 
		int* pointer = &number;		라고 선언 했었습니다. 앞의 int* 을 다시 살펴보자면
		- * : 이 변수는 포인터 타입이다! (8바이트 크기) 주소를 담는 바구니!
		- int : 주소를 따라가면 int(4바이트 정수형 바구니)가 있다고 가정하고 분석해라!
	
		[!] 포인터에서 +나 -등 산술 연산으로 1을 더하거나 빼면,
		정말 '그 숫자'를 더하고 빼라는 의미가 아니다!
		한번에 TYPE의 크기만큼 이동하라!
		다음/이전 바구니로 이동하고 싶다 << [바구니 단위]의 이동으로
		즉, 1을 더하면 = 바구니 1개 크기 만큼 이동시켜라
		3을 더하면 = 바구니 3개 크기 만큼 이동시켜라
		*/
	
	// 3) 간접 연산자 (*)
		// 이전에 비유하기를 포탈을 타고 해당 주소로 슝~ 이동한다고 했습니다.
		* pointer = 3;  // number = 3;  과 같은 의미

	// 4) 간접 멤버 연산자 (->)

		// 이전에 구조체(struct)로 비슷한 변수들을 묶어 관리 했었습니다.
		Player player;
		player.hp = 100;
		player.damage = 20;
		
		// 포인터를 구조체에도 적용할 수 있습니다.
		Player* playerPtr = &player;

		// 그러면 간접연산자를 사용해 보겠습니다.
		(*playerPtr).hp = 200;
		(*playerPtr).damage = 50;
		// 어셈블리상에서 까보면 결국 *playerPtr 의 두가지 내용물중 hp은 + 0 의 주소에 있다는걸 알 수 있고
		// damage는 +4 의 위치에 있다는걸 알수 있었습니다.

		// 위 문법중 괄호나 *이나 . 을 합쳐서 간편하게 표현하는게 
		// 간접 멤버 연산자 (->) 입니다.
		playerPtr->hp = 300;
		playerPtr->damage = 100;
		// 위의 문법과 같은 의미입니다.
		// * 은 간접 연산자로 (포탈타고 해당 주소로 GO GO)
		// . 은 구조체의 특정 멤버를 다룰때 사용 (어셈블리에서 보면 사실상 덧셈의 역할)

	
	return 0;
}