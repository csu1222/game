#include <iostream>
using namespace std;

// 오늘의 주제 : 포인터 실습


struct StatInfo		// 총 12바이트 크기가 될것입니다.
{
	int hp;		// +0
	int attack;	// +4
	int defence;// +8
};
/*
이전에는 
int hp;			// +0
int attack;		// +4
int defence;	// +8
이렇게 변수를 만들었습니다.
어셈블리상에서는 struct 나 개별로 만드나 똑같았습니다.
*/

void EnterLobby();
StatInfo CreatePlayer();				// ouput을 StatInfo 타입을 뱉는 방식
void CreateMonster(StatInfo* info);		// 매개변수로 StatInfo 포인터를 받는 방식
// bool 인 이유는 플레이어가 승리 했는지 안했는지 뱉어 줄 것이기 때문
bool StartBattle(StatInfo* player, StatInfo* monster);  

int main()
{
	EnterLobby();

	return 0;
}

void EnterLobby()
{
	cout << "로비에 입장했습니다." << endl;

	StatInfo player;
	player.hp = 0xbbbbbbbb;			// 이 부분은 디버깅 할때 눈에 잘띄기 위해서 엉뚱한 값을 넣어 놓은 겁니다.
	player.attack = 0xbbbbbbbb;
	player.defence = 0xbbbbbbbb;

	// [매개변수][RET][지역변수(temp(100, 10, 2) player (b,b,b,...)]  [매개변수][RET][지역변수(ret(100, 10, 2))]
	// CreatePlayer 함수는 아웃풋으로 StatInfo 구조체를 뱉는 함수 였습니다. CreatePlayer가 player 변수에 값을 
	// 리턴 해주는 공정을 살펴보면
	// 1) 위의 hp, attack, defence 값들은 제자리에 저장이 되있는걸 확인 했습니다.
	// 2) CreatePlayer함수를 실행하기전에 스택의 어떤 주소를 push 해주었습니다. 이 지점을 임의로 temp라고 하겠습니다.
	// 3) push를 해주었다는 것은 CreatePlayer함수의 매개변수로 주었다는 의미입니다.
	// 4) CreatePlayer함수내 지역변수 ebp-8, ebp-12m ebp-16에 각각 defence값, attack값, hp값을 넣어주었습니다.
	// 5) CreatePlayer함수의 스택에서 ebp+8 값이 가리키는 주소로 각 ret.hp, ret.attack, ret.defence 를 차례로 넣어주었습니다.
	// 6) 위 ebp+8은 임의로 이름 지었던 temp의 주소였습니다. 이 후 진행하다 보면 ret 명령어로 어딘가에 이동됩니다.
	// 7) 이후 temp에 복사된 값들을 player구조체의 각각의 값에 넣어주었습니다.

	// 요약하자면 
	// 1) 함수 실행시 temp라는 임의의 주소에 값들을 저장하고 
	// 2) 그 값을 다시 player에 복사해주었습니다.
	// temp라는 임의의 주소를 거쳐서 값을 리턴 받는게 왠지 비효율적으로 보입니다.

	// 지금은 예시로 Info 값이 3개 밖에 없지만 실제 서비스되는 게임에서는 Info의 크기가 몇천 바이트가 될 수도 있다고 합니다.
	// 그럴때 지금처럼 지역변수에 값을 생성하고, 매개변수로 받은곳에 복사하고, 그 매개변수의 값을 다시 원래 변수에 복사하고,
	// 하는 과정을 거치면 성능적으로 많은 손해를 보게 될 수 있습니다.
	player = CreatePlayer();


	
	StatInfo monster;
	monster.hp = 0xbbbbbbbb;			// 이 부분은 디버깅 할때 눈에 잘띄기 위해서 엉뚱한 값을 넣어 놓은 겁니다.
	monster.attack = 0xbbbbbbbb;
	monster.defence = 0xbbbbbbbb;

	// 이번에는 CreateMonster를 살펴보겠습니다. CreateMonster는 포인터를 이용해서 구현 했었습니다.
	// 1) monster의 값들이 각각 bbbb로 되어있는걸 확인했습니다.
	// 2) CreatePlayer때 처럼 임의의 temp주소가 아닌 monster변수를 CreateMonster의 매개변수로 push 해주었습니다.
	// 3) 이제 CreateMonster함수 내부로 들어가서 보니 값을 설정해 줄 때 매개변수로 받은 monster변수의 값을 조작되었습니다.
	
	CreateMonster(&monster);

	// CratePlayer 처럼 리턴값으로 값을 복사하나 CreateMonster로 포인터를 직접 받아 값을 설정하나
	// 결과는 같았지만, 속도면에서 많은 차이가 있었습니다.


	// 번외편 1)
	// 구조체 끼리 복사 할 때 무슨일이 벌어질까?
	/*player = monster;
	
	player.hp = monster.hp;
	player.attack = monster.attack;
	player.defence = monster.defence;*/
	// 위의 코드와 아래 일일히 값을 넣어주는 코드가 사실상 같은 동작을 하고있습니다.

	bool victory = StartBattle(&player, &monster);

	if (victory)
		cout << "VICTORY!" << endl;
	else
		cout << "YOU LOSE!" << endl;
}

StatInfo CreatePlayer()
{
	StatInfo ret;

	cout << "플레이어 생성!" << endl;

	ret.hp = 100;
	ret.attack = 10;
	ret.defence = 2;

	return ret;
}

void CreateMonster(StatInfo* info)
{

	cout << "몬스터 생성!" << endl;

	info->hp = 40;
	info->attack = 8;
	info->defence = 1;
}

bool StartBattle(StatInfo* player, StatInfo* monster)
{
	while (true)
	{
		int damage = player->attack - monster->defence;
		if (damage < 0)
			damage = 0; 

		monster->hp -= damage;
		if (monster->hp < 0)
			monster->hp = 0;
		cout << "monster's HP : " << monster->hp << endl;

		if (monster->hp == 0)
			return true;

		damage = monster->attack - player->defence;
		if (damage < 0)
			damage = 0;

		cout << "player's HP : " << monster->hp << endl;

		player->hp -= damage;
		if (player->hp < 0)
			player->hp = 0;

		if (player->hp == 0)
			return false;
	}
}