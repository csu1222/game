#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>

// 오늘의 주제 : 정렬 


// 

//1) 버블 정렬(Bubble Sort)
void BubbleSort(vector<int>& v)
{
	const int n = (int)v.size();

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < (n - 1 - i); j++)
		{
			if (v[j] > v[j + 1])
			{
				int temp = v[j];
				v[j] = v[j + 1];
				v[j + 1] = temp;
			}
		}
	}
}
//2) 선택 정렬(Selection Sort)

void SelectionSort(vector<int>& v)
{
	const int n = (int)v.size();

	// 승자를 찾는 루프
	for (int i = 0; i < n - 1; i++)
	{	
		// 일단 최선의 인덱스는 시작 인덱스
		int bestIdx = i;

		// 스캔을 돌면서 맨처음 원소보다 좋은 후보가 있는지 찾음
		for (int j = i + 1; j < n ; j++)
		{
			if (v[j] < v[bestIdx])
				bestIdx = j;
		}

		// 찾은 최선의 후보를 앞에서부터 채워넣음
		int temp = v[i];
		v[i] = v[bestIdx];
		v[bestIdx] = temp;
	}
}

//3) 삽입 정렬(Insertion Sort)

void InsertionSort(vector<int>& v)
{
	const int n = v.size();

	// 첫번째 원소는 딱히 연산할게 없으니까 두번째 원소부터 연산 시작 
	for (int i = 1; i < n; i++)
	{	
		// 매 반복마다 삽입할 데이터를 고릅니다.
		int insertData = v[i];

		// j 는 InsertData 의 앞 순서부터 거꾸로 순회를 돕니다.
		int j;
		for (j = i - 1; j >= 0; j--)
		{
			if (v[j] > insertData)
				v[j + 1] = v[j];
			else
				break;
		}
		v[j + 1] = insertData;
	}
}

int main()
{
	vector<int> v{ 1,3,5,2,4 };

	//BubbleSort(v);
	//SelectionSort(v);
	InsertionSort(v);

}