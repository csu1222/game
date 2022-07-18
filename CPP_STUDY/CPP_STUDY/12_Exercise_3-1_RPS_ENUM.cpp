#include <iostream>
using namespace std;


// 이전 가위 바위 보 프로그램에서 const int 를 사용했었습니다.
////const int SCISSORS = 1;
////const int ROCK = 2;
////const int PAPER = 3;

// 사용할 상수가 몇개 없으면 괜찮지만 상수 갯수가 늘면 
// 한번에 관리할 필요가 생깁니다.
// 그럴 때 '열거형' enum명령어를 사용합니다.
// 초기 값을 따로 설정 하지 않으면 0 부터 차례대로 값이 부여됩니다.

enum ENUM_SRP  //enum 명령어 옆에 이름을 붙힐 수 있습니다.
{
	ENUM_SCISSORS = 1,		// 처음에 값을 1로 설정 하였으니 다음 변수에는 1, 2, 3 순으로 값이 부여될 것입니다.
	ENUM_ROCK,
	ENUM_PAPER
};

// const 와 enum 으로 만든 상수의 차이점!
// const의 경우 선언된 장소가 함수 밖인지 함수안 인지 등 의 조건에 따라 컴파일러가 
// - 변수를 메모리에 저장 해 놨을 수도 있고 아니면 컴파일하는 순간에 그냥 값으로 치환 될 수도 있습니다.
// enum의 경우 무조건 컴파일시 해당 값으로 치환되서 컴파일 됩니다.
// - 그래서 아무리 enum의 갯수를 늘려도 메모리에 잡히는게 아닌 코드의 가독성만을 높힐 수 있습니다.


// #define 문
// - #이 붙은 거는 -> 전처리 지시문
// 1) 전처리  2) 컴파일 3) 링크
// define은 전처리 단계에서 자동으로 값을 치환 해주는 메크로 라고 볼 수 있습니다.
// - enum 과의 차이점 enum은 정수값만 치환 할 수 있고 갯수를 늘리기 편하지만
// - define은 정수이외의 것들도 치환 할 수 있습니다.
#define DEFINE_SCISSORS 1
#define DEFINE_TEST	cout << "hello world" << endl;


// 번외- #include 문 
// - 지금껏 c++ 파일을 시작 할 때 #include <iostream> 을 입력 했었는데
// - 그때 #include <파일명> 은 파일명에 해당하는 파일을 통째로 복사해서 이 파일에 붙혀넣는 기능입니다.

int main()
{	
	DEFINE_TEST

	int wins = 0;
	int total = 0;

	srand(time(0));

	while (true) // 프로그램을 지속적으로 유지 시키기 위해 일부러 무한 루프를 걸어줍니다.
	{

		cout << "가위(1) 바위(2) 보(3) 중 하나를 고르세요" << endl;
		cout << ">";

		if (total == 0)
		{
			cout << "승률 : 없음" << endl;
		}
		else
		{
			int winPer = (wins * 100) / total;
			cout << "승률 : " << winPer << endl;
		}

		int input;
		cin >> input;

		int computerValue = 1 + (rand() %3);

		if (input == ENUM_SCISSORS)
		{
			switch (computerValue)
			{
			case ENUM_SCISSORS:
				cout << "가위(유저) vs 가위(컴퓨터) 비겼습니다!" << endl;
				break;
			case ENUM_ROCK:
				total++;
				cout << "가위(유저) vs 바위(컴퓨터) 졌습니다!" << endl;
				break;
			case ENUM_PAPER:
				wins++;
				total++;
				cout << "가위(유저) vs 보(컴퓨터) 이겼습니다!" << endl;
				break;
			}
		}
		else if (input == ENUM_ROCK)
		{
			switch (computerValue)
			{
			case ENUM_SCISSORS:
				wins++;
				total++;
				cout << "바위(유저) vs 가위(컴퓨터) 이겼습니다!" << endl;
				break;
			case ENUM_ROCK:
				cout << "바위(유저) vs 바위(컴퓨터) 비겼습니다!" << endl;
				break;
			case ENUM_PAPER:
				total++;
				cout << "바위(유저) vs 보(컴퓨터) 졌습니다!" << endl;
				break;
			}

		}
		else if (input == ENUM_PAPER)
		{
			switch (computerValue)
			{
			case ENUM_SCISSORS:
				total++;
				cout << "보(유저) vs 가위(컴퓨터) 졌습니다!" << endl;
				break;
			case ENUM_ROCK:
				wins++;
				total++;
				cout << "보(유저) vs 바위(컴퓨터) 이겼습니다!" << endl;
				break;
			case ENUM_PAPER:
				cout << "보(유저) vs 보(컴퓨터) 비겼습니다!" << endl;
				break;
			}

		}
		else 
		{
			break;
		}
	}
}