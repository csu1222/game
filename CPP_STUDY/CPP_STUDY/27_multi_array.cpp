#include <iostream>
using namespace std;

// 오늘의 주제 : 다차원 배열





int main()
{
	// 배열이라 함은
	int a[10] = { 1, 2, 3 };
	// TYPE 과 배열이름, 크기 그리고 초기화 값


	// 아파트로 예를들면

	// [1] [1] [5] [2] [2] << 1층
	// [4] [2] [3] [4] [1] << 0층

	int first[5] = { 4, 2, 3, 4, 1 };
	int second[5] = { 1, 1, 5, 2, 2 };
	// 아파트가 얼마나 높아질지 모르는 상태에서 이렇게 변수를 늘리는것은 좋은 방법이 아닐겁니다.

	// 다차원 배열 선언법
	int apartment2D[2][5] = { { 4, 2, 3, 4, 1 } , { 1, 1, 5, 2, 2 } };
	// 값을 꺼내는 법
	// apartment2D[0~1][0~4]

	for (int floor = 0; floor < 2; floor++)
	{
		for (int room = 0; room < 5; room++)
		{
			// 주소 : apartment2D + (floor * 20) + 4*room
			int num = apartment2D[floor][room];

			cout << num << " ";
		}
		cout << endl;
	}
	// 디버그 도구로 메모리를 살펴본 결과 정수 5개 짜리 배열이 따로 있는게 아닌 
	// 한번에 모여서 정수 10개가 있었습니다.
	// 그러면 1차원 배열로 똑같이 만들수 있을까?

	int apartment1D[10] = { 4, 2, 3, 4, 1, 1, 1, 5, 2, 2 };

	for (int floor = 0; floor < 2; floor++)
	{
		for (int room = 0; room < 5; room++)
		{
			// 주소 : apartment1D + (floor * 20) + 4*room  int형으로 이루어진 배열들 이기 때문에
			int index = (floor * 5) + room;
			int num = apartment1D[index];

			cout << num << " ";
		}
		cout << endl;
	}

	// 2차 배열은 언제 사용할까? 대표적으로 2D 로그라이크 맵
	int map[5][5] =
	{
		{1, 1, 1, 1, 1},
		{1, 0, 1, 1, 1},
		{0, 0, 1, 1, 1},
		{1, 0, 1, 1, 1},
		{1, 1, 1, 1, 1}
	};
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			int info = map[y][x];

			cout << info;

		}
		cout << endl;
	}

	return 0;
}