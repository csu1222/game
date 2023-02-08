#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>

// 오늘의 주제 : 퀵 정렬 Quick Sort


// [5][1][3][7][9][2][4][6][8]
//  p
//    low				   high
int Partition(vector<int>& v, int left, int right)
{
	// 피벗 , 로우, 하이 
	int pivot = v[left];
	int low = left + 1;
	int high = right;

	// low 와 high 가 교차되는 때 까지 반복 
	while (low <= high)
	{
		// 1 단계
		// - pivot >= arr[low] 일 동안 low 를 오른쪽으로 이동
		while (low <= right && pivot >= v[low])
			low++;

		// - pivot <= arr[high] 일 동안 high 를 왼쪽으로 이동
		while (high >= left + 1 && pivot <= v[high])
			high--;

		// 2 단계
		// - low < high 라면 arr[low]와 arr[high] 데이터 교체
		if (low < high)
			swap(v[low], v[high]);
	}

	// 3 단계
	// - high <= low 면 빠져나오고, pivot과 arr[high] 교체
	swap(v[left], v[high]);

	return high;
}

void QuickSort(vector<int>& v, int left, int right)
{
	if (left > right)
		return;
	
	int pivot = Partition(v, left, right);

	QuickSort(v, left, pivot - 1);
	QuickSort(v, pivot + 1, right);
}

int main()
{
	vector<int> v;

	srand(static_cast<int>(time(0)));


	for (int i = 0; i < 50; i++)
	{
		int randValue = rand() % 100;

		v.push_back(randValue);
	}

	QuickSort(v, 0, v.size() - 1);
}