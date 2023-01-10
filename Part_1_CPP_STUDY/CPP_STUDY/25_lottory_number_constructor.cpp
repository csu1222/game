#include <iostream>
using namespace std;

// 오늘의 주제 : 로또 번호 생성기


// 1) 스왑
void Swap(int& a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

	//	정렬 실습 (실패 미지의 오류)
//void Sort(int numbers[], int count)
//{
//	int temp;
//	for (int i = 0; i < count; i++)
//	{
//		for (int j = i; j < count; j++)
//		{
//			if (numbers[j] < numbers[j + 1])
//				continue;
//			else
//			{
//				Swap(numbers[j], numbers[j + 1]);
//			}
//		}
//	}
//}


// 2) 정렬하기
void Sort(int numbers[], int count)
{
	for (int i = 0; i < count; i++)
	{
		// i 번째가 제일 좋은 후보라고 가정
		int best = i;
		
		// 다른 후보와 비교를 통해 제일 좋은 후보를 찾아 나선다
		for (int j = i + 1; j < count; j++)
		{
			if (numbers[j] < numbers[best])
				best = j;
		}

		// 제일 좋은 후보와 교체하는 과정
		if (i != best)
			Swap(numbers[i], numbers[best]);
	}
}

// 3) 로또 번호 생성기
void ChooseLotto(int numbers[])
{
	// TODO : 랜덤으로 1~45 사이의 숫자 6개를 골라주세요! (단, 중복이 없어야 함)
	int count = 0;
	while (count < 6)
	{
		int landValue = 1 + (rand() % 45);

		// 이미 찾은 값인지?
		bool found = false;
		for (int i = 0; i < count; i++)
		{
			if (numbers[i] == landValue)
			{
				// 이미 찾은 값
				found = true;
				break;
			}
		}

		// 못 찾았으면 추가!
		if (found == false)
		{
			numbers[count] = landValue;
			count++;
		}
	}

	Sort(numbers, 6);
}

void ChooseLotto2(int numbers[])
{
	for (int count = 0; count != 6;)
	{

		int landValue = 1 + (rand() % 45);

		// 이미 찾은 값인지?
		bool found = false;
		for (int i = 0; i < count; i++)
		{
			if (numbers[i] == landValue)
			{
				// 이미 찾은 값
				found = true;
				break;
			}
		}

		// 못 찾았으면 추가!
		if (found == false)
		{
			numbers[count] = landValue;
			count++;
		}
	}
	Sort(numbers, 6);
}


int main()
{
	srand((unsigned)time(0));
	// 1) Swap 함수 만들기 
	int a = 1;
	int b = 2;
	cout << "스왑 전 a:" << a << " b:" << b << endl;
	Swap(a, b);
	cout << "스왑 후 a:" << a << " b:" << b << endl;

	// 2) 정렬 함수 만들기 (작은 숫자가 먼저 오도록 정렬)
	int numbers[] = { 1, 42, 3, 15, 5, 6 };
	Sort(numbers, sizeof(numbers) / sizeof(int));

	// 3) 로또 번호 생성기
	ChooseLotto2(numbers);

	for (int i = 0; i < (sizeof(numbers) / sizeof(int)); i++)
	{
		cout << "|" << numbers[i] << "|" << endl;
	}

	return 0;
}