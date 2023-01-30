#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 이진 탐색 트리 

// 이진 탐색
// -상황) 배열에 데이터가 정렬되어 있다.

// -준비된 데이터 
// -[1][8][15][23][39][47][56][61][81][91]

// -Q) 82 라는 숫자가 배열에 있습니까?

// -A1) 0번 인덱스부터 하나씩 순회해서 탐색 - 시간 복잡도 = O(N)

// 정렬된 배열을 이용해서 이진 탐색 가능 (numbers[mid])
// - 잠깐! 그런데 배열의 단점이 뭐더라?
// -- 중간 삽입/삭제가 느리다!
// 정렬된 연결 리스트로는 불가 (임의 접근 X)

vector<int> numbers;

void BinarySearch(int n)
{
	// TODO 
	// 인자로 받은 숫자가 numbers 안에 있는지를 탐색 후 있다면 로그로 출력

	int left = 0;
	int right = (int)numbers.size() - 1;

	while (left <= right)
	{
		cout << "탐색범위: " << left << " 에서 " << right << endl;
		int mid = (left + right) / 2;
		
		if (n < numbers[mid])
		{
			cout << n << " < " << numbers[mid] << endl;
			right = mid - 1;
		}
		else if (n > numbers[mid])
		{
			cout << n << " > " << numbers[mid] << endl;
			left = mid + 1;
		}
		else 
		{
			cout << n << " = " << numbers[mid] << endl;
			cout << "찾음!" << endl;
			break;
		}
	}
}

int main()
{
	numbers = vector<int>{ 1,8,15,23,39,47,56,61,81,91 };

	BinarySearch(82);
}