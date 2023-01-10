#include <iostream>
using namespace std;

// 오늘의 주제 : 배열

struct StatInfo
{
	int hp = 0xAAAAAAAA;
	int attack = 0xBBBBBBBB;
	int defence = 0xDDDDDDDD;
};


int main()
{	
	// TYPE 이름[개수];

	// 배열의 크기는 상수여야 함 (Visual Studio 기준)
	const int monstersCount = 10;
	StatInfo monsters[monstersCount];

	// 여태껏 변수들의 [이름]은 바구니의 이름이었음
	
	// 그런데 배열은 [이름] 조금 다르게 동작한다
	StatInfo players[10];
	// players = monsters;    변수였으면 값을 넣을 수 있었는데 오류가 납니다.

	// 그럼 배열의 이름은 뭐지?
	// 배열의 이름은 곧 배열의 시작 주소
	// 정확히는 시작 위치를 가리키는 TYPE* 포인터

	auto WhoAmI = monsters;  // auto 는 C++ 11 부터 생긴 문법으로 상대의 타입을 유추하여 타입을 자동으로 바꿔주는 치트키 같은 타입
	// auto로 monsters라는 배열의 타입을 알아본 결과 StatInfo* 즉, StatInfo타입의 포인터 였습니다.
	
	// 포인터에 대해 배웠듯이 첫번째 요소에 대해 수정을 해봤습니다.
	StatInfo* monster_0 = monsters;
	monster_0->hp = 100;
	monster_0->attack = 10;
	monster_0->defence = 1;

	// 그렇다면 두번째... 그리고 다음 요소들은 어떻게 접근할까요?
	// 포인터의 산술연산을 이용 포인터에 +1 은 타입의 크기만큼씩 움직입니다.
	StatInfo* monster_1 = monsters + 1;
	monster_1->hp = 200;
	monster_1->attack = 20;
	monster_1->defence = 2;

	// 포인터와 참조는 자유자재로 변환 가능하다 
	StatInfo& monster_2 = *(monsters + 2);
	monster_2.hp = 300;
	monster_2.attack = 30;
	monster_2.defence = 3;

	// [주의] 이거는 완전 다른의미다
	// &가 붙어있나 안 붙어 있나 차이지만 이 코드는 monsters+2 의 주소에 있는 구조를 복사해서 
	// 별개의 변수를 만든것
	StatInfo temp = *(monsters + 2);
	monster_2.hp = 300;
	monster_2.attack = 30;
	monster_2.defence = 3;

	// 반복문을 사용해 자동화한다!
	for (int i = 0; i < 10; i++)
	{
		// 잠깐!) 같은 변수이름을 계속 사용할 수 있는 이유는 반복 시행마다 변수의 유효범위가 끝나므로 다시 사용할 수 있습니다. 
		StatInfo& monster = *(monsters + i);
		monster.hp = 100 * (i + 1);
		monster.attack = 10 * (i + 1);
		monster.defence = (i + 1);
	}
	

	// 근데 *(monsters + i) 너무 불편하고 가독성이 떨어진다 ... 더 편한 방법?
	// 인덱스! 
	// 배열은 0번부터 시작. N번째 인덱스에 해당하는 데이터에 접근하려면 배열이름[N]
	// *(monsters = i) = monsters[i]

	// 이렇게 간편하게 배열의 요소들에 접근 할 수 있습니다.
	monsters[0].hp = 100;
	monsters[0].attack = 10;
	monsters[0].defence = 1;

	for (int i = 0; i < 10; i++)
	{ 
		monsters[i].hp = 100 * (i + 1);
		monsters[i].attack = 10 * (i + 1);
		monsters[i].defence = (i + 1);
	}
	// 값을 꺼낼 때도
	int a = monsters[0].hp;

	// 배열 초기화 문법 몇가지
	int numbers[5] = {};  // 0으로 초기화
	int numbers1[10] = { 1, 2, 3, 4, 5 };  // 설정한 애들은 설정한 값들로, 나머지 값들은 0을 초기와
	int numbers2[] = { 1, 2, 3, 4, 5, 6 };  // 데이터 갯수 만큼의 크기에 해당하는 배열로 만들어준다.
	char hellostr[] = { 'H', 'e', 'l', 'l', 'o', '\0' };
	cout << hellostr << endl;


	// 배열 요약> :
	// 1) 선언한다
	int arr[10] = { };

	// 2)  인덱스로 접근해서 사용
	arr[1] = 1;




	return 0;
}