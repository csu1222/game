#include <iostream>
using namespace std;

// 연습문제 : 별찍기, 구구단


int main()
{	
#pragma region cout, cin
	// 선수지식으로
	// 여태껏 쓰던 cout과 인풋을 받는 새로운 명령어 cin
	//int input;
	//cin >> input;

	//cout << "hello world" << endl;
	// 맨뒤의 endl; 는 end line 으로 이 줄을 끝내고 다음줄로 넘어간다는 의미
	// cout << "hello world\n";		으로 표현해도 상관없습니다.
#pragma endregion

#pragma region 별찍기
	// 유저들이 어떤 정수를 입력하면 
	// N*N개의 별을 찍었으면 좋겠어요

	int input;
	cin >> input;

	for (int t = 0; t < input; t++)
	{
		for (int i = 0; i < input; i++)
		{
			cout << "*";
		}
		cout << endl;
	}
#pragma endregion

#pragma region 별찍기 2
	// 유저들이 어떤 정수를 입력하면 
	// N줄의 별을 계단식으로 출력해주세요

	int input2;
	cin >> input2;

	for (int t = 0; t < input2; t++)
	{
		for (int i = 0; i < t + 1; i++)
		{
			cout << "*";
		}
		cout << endl;
		
	}
#pragma endregion

#pragma region 별찍기 3
	// 유저들이 어떤 정수를 입력하면 
	// N개 부터 시작해서 줄마다 1개씩 줄어드는 형태로!

	int input3;
	cin >> input3;

	for (int t = 0; t < input3; t++)
	{
		for (int i = 0; i < (input3 - t); i++)
		{
			cout << "*";
		}
		cout << endl;

	}
#pragma endregion


}