#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>

// 오늘의 주제 : 힙 정렬, 병합 정렬

void HeapSort(vector<int>& v)
{
	priority_queue<int, vector<int>, greater<int>> pq;

	for (int num : v)
		pq.push(num);

	v.clear();

	while (pq.empty() == false)
	{
		v.push_back(pq.top());
		pq.pop();
	}
}

void MergeResult(vector<int>& v, int left, int mid, int right)
{
	int leftIdx = left;
	int rightIdx = mid + 1;

	vector<int> temp;

	while (leftIdx <= mid && rightIdx <= right)
	{
		if (v[leftIdx] <= v[rightIdx])
		{
			temp.push_back(v[leftIdx]);
			leftIdx++;
		}
		else
		{
			temp.push_back(v[rightIdx]);
			rightIdx++;
		}
	}
	
	// 위의 while 문을 빠져 나왔다고 끝난게 아니라 두 덩어리중 한 덩어리가 먼저 끝났다는 이야기

	// 남은 데이터들을 다 push 합니다. 

	// 왼쪽이 먼저 끝났으면, 오른쪽의 나머지 데이터 복사
	if (leftIdx > mid)
	{
		while (rightIdx <= right)
		{
			temp.push_back(v[rightIdx]);
			rightIdx++;
		}
	}
	// 오른쪽이 먼저 끝났으면, 왼쪽의 나머지 데이터 복사
	else
	{
		while (leftIdx <= mid)
		{
			temp.push_back(v[leftIdx]);
			leftIdx++;
		}
	}

	// v 에 temp 를 하나씩 복사합니다. 그런데 v의 모든 영역을 항상 사용할 수 있는건아니고 
	// 인자로 받은 left 의 위치 부터 복사해나가야 하는데 그 이유는 
	// 현재 재귀 함수로 영역을 찝어서 진행하고 있기 때문입니다. 
	for (int i = 0; i < temp.size(); i++)
		v[left + i] = temp[i];

}

void MergeSort(vector<int>& v, int left, int right)
{
	if (left >= right)
		return;

	int mid = (left + right) / 2;

	MergeSort(v, left, mid);
	MergeSort(v, mid + 1, right);

	MergeResult(v, left, mid, right);
}

vector<int> Merge(vector<int>& a, vector<int>& b)
{
	int aIdx = 0;
	int bIdx = 0;

	vector<int> tem2;

	while (aIdx <= a.size() - 1 && bIdx <= b.size() - 1)
	{
		if (a[aIdx] <= b[bIdx])
		{
			tem2.push_back(a[aIdx]);
			aIdx++;
		}
		else
		{
			tem2.push_back(b[bIdx]);
			bIdx++;
		}

	}

	if (aIdx < a.size() - 1)
	{
		while (aIdx <= a.size() - 1)
		{
			tem2.push_back(a[aIdx]);
			aIdx++;
		}
	}
	else
	{
		while (bIdx <= b.size() - 1)
		{
			tem2.push_back(b[bIdx]);
			bIdx++;
		}
	}
	vector<int> tem = {};
	for (int i = 0; i < tem2.size(); i++)
	{
		tem.push_back(tem2[i]);
	}

	return tem;
}

int main()
{
	vector<int> v;
	
	srand(time(0));

	for (int i = 0; i < 50; i++)
	{
		int randValue = rand() % 100;
		v.push_back(randValue);
	}

	// HeapSort(v);
	MergeSort(v, 0, v.size() - 1);


	vector<int> a;
	vector<int> b;

	for (int i = 0; i < 25; i++)
	{
		int aValue = rand() % 25;
		int bValue = rand() % 25;
		a.push_back(aValue);
		b.push_back(bValue);
	} 

	MergeSort(a, 0, a.size()-1);
	MergeSort(b, 0, b.size()-1);


	vector<int> result = Merge(a, b);
}