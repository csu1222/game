#include <iostream>
using namespace std;

// 가위 바위 보 프로그램

// 참고) 랜덤한 수 생성하는법
// srand()라는 함수로 시드값을 생성하고 
// rand() 함수로 시드에 따라 난수를 생성합니다.
// srand(time()) 으로 보통 현재 시간을 인수로 시드를 생성합니다.

int main()
{
	srand(time(0));

//	rand();  // 0 ~ 32767 사이의 값을 출력

	// 가위 바위 보 프로그램에서는 1,2,3 의 값만 필요
	//int value = 1 + (rand() % 3);		// 3으로 나눈 나머지는 0, 1, 2 세값중 하나일 것입니다.

	const int SCISSORS = 1;
	const int ROCK = 2;
	const int PAPER = 3;

	// 추가 사항으로 가위바위보 승률을 구하고 싶습니다.
	int win = 0;
	int round = 0;

	
	while (true)
	{
		cout << "가위(1) 바위(2) 보(3) 중 하나를 골라주세요!" << endl;
		cout << ">";

		if (round == 0)
		{
			cout << "승률 : 없음" << endl;
		}
		else
		{
			// 첫 승률 구하기 시도에서는 제대로 구해지지 않고 0만 출력되었습니다.
			//int winPer = (win / round) * 100;

			// 나누기를 할 때 유의 사항에서 정수 나누기 정수는  1/4 가 0.25지만 정수만 다루기 때문에 0이 됩니다
			// 그래서 win / round * 100 은 앞부분에 소수점이 오기 때문에 
			// (win * 100) / round 로 해결 할 수 있었습니다.
			int winPer = (win * 100) / round;
			cout << "승률 : " << winPer << endl;

			
		}
		int computerVlaue = 1 + (rand() % 3);

		int input;
		cin >> input;


		// 각 가위 바위 보 결과에 따라 적절히 win과 round에 1씩 추가했습니다.
		if (input == SCISSORS)
		{
			switch (computerVlaue)
			{
			case SCISSORS:
				cout << "가위(사용자) vs 가위(컴퓨터) 비겼습니다!" << endl;
				break;
			case ROCK:
				round++;
				cout << "가위(사용자) vs 바위(컴퓨터) 졌습니다!" << endl;
				break;
			case PAPER:
				win++;
				round++;
				cout << "가위(사용자) vs 보(컴퓨터) 이겼습니다!" << endl;

				break;
			}
		}
		else if (input == ROCK)
		{

			switch (computerVlaue)
			{
			case SCISSORS:
				win++;
				round++;
				cout << "바위(사용자) vs 가위(컴퓨터) 이겼습니다!" << endl;
				break;
			case ROCK:
				cout << "바위(사용자) vs 바위(컴퓨터) 비겼습니다!" << endl;
				break;
			case PAPER:
				round++;
				cout << "바위(사용자) vs 보(컴퓨터) 졌습니다!" << endl;
				break;
			}
		}
		else if (input == PAPER)
		{

			switch (computerVlaue)
			{
			case SCISSORS:
				round++;
				cout << "보(사용자) vs 가위(컴퓨터) 졌습니다!" << endl;
				break;
			case ROCK:
				win++;
				round++;
				cout << "보(사용자) vs 바위(컴퓨터) 이겼습니다!" << endl;
				break;
			case PAPER:
				cout << "보(사용자) vs 보(컴퓨터) 비겼습니다!" << endl;
				break;
			}
		}
		else
		{
			break;
		}
	}


}