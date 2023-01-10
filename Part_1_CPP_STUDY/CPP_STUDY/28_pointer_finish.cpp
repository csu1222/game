#include <iostream>
using namespace std;

// 오늘의 주제 : 포인터 마무리

// 1) 포인터 vs 배열 2탄
// 2) 주의사항(마음가짐?)

// 포인터나 참조를 응용하면서 리턴값으로 주면 어떨까?
int& TestRef()
{
	int a = 1;
	return a;
}

int* TestPtr()
{
	int a = 1;
	return &a;
}

void TestWrong(int* ptr)
{
	int a[100] = {};
	a[99] = 0xAAAAAAAA;
	*ptr = 0x12341234;
}

int main()
{
	// 주소를 담는 바구니
	// 진퉁은 저~멀리 어딘가에 있음
	// p는 단지 그 곳으로 워프하는 포탈
	int* p;

	// 진짜배기! 원조 데이터
	// 닭장 처럼 데이터의 묶음 (엄청 많고 거대함)
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8 };

	// 그런데 상당히 많은 사람들이 [배열 = 포인터]라 착각하는 경향이 있음!

	// - [배열의 이름]은 배열의 시작 주소값을 가리키는 TYPE* 포인터로 변환 가능!
	p = arr;

	// - [TYPE형 1차원 배열]과  [TYPE* 포인터]는 완전히 호환이 된다.
	cout << p[0] << endl;
	cout << arr[0] << endl;
	cout << p[5] << endl;
	cout << arr[5] << endl;

	cout << *p << endl;
	cout << *arr << endl;
	cout << *(p + 3) << endl;
	cout << *(arr + 3) << endl;

	// 지옥을 보여드리겠습니다. ( 2차원 배열 vs 다중 포인터 )

	// [1] [2] [3] [4]
	int arr2[2][2] = { {1, 2}, {3, 4} };

	// pp에 억지로 arr2를 형변환 해서 넣어 봤습니다.
	// 주소2 [ ] << 4바이트
	// 주소1[ 00000001 ]
	// pp [ 주소1 ]

	// [ ][ ]
	// [ 주소 ]
	int(*p2)[2] = arr2;
	cout << (*p2)[0] << endl;
	cout << (*p2)[1] << endl;
	cout << (*(p2 + 1))[0] << endl;
	cout << (*(p2 + 1))[1] << endl;

	cout << p2[0][0] << endl;
	cout << p2[0][1] << endl;
	cout << p2[1][0] << endl;
	cout << p2[1][1] << endl;

	// 2) 함수의 리턴값으로 참조나 포인터를 주는 경우
	int* pointer = TestPtr();

	// 문제없이 실행 되지만...

	TestWrong(pointer);
	// 함수내의 스택영역에 있는 포인터, 참조를 함부로 건내주면 다시 다른 함수를 불러올때 메모리가 오염될 수 있습니다.



	return 0;
}