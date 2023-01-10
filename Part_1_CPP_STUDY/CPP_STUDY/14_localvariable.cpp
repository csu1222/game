#include <iostream>
using namespace std;

// 오늘의 주제 : 지역 변수 와 값 전달

// 지역 변수를 배우기 위해 반대 되는 개념인 전역 변수에 대해 알아보겠습니다.
// 전역 변수
// - 함수 밖에 정의된 변수
// - 전역변수는 이 파일 내에 어느곳에서도 가져다 쓸 수 있습니다.
// - const여부 나 초기화 여부 에 따라 .rodata, .data, .bss 영역에 저장됩니다. 

int GloblaVariable = 0;
// 함수내에 전역변수를 가져다 쓰는 test
void Test()
{
	cout << "전역 변수의 값은 : " << GloblaVariable << endl;
	GloblaVariable++;
}


// 1) 지역 변수에 관한 예제
void IncreseHp(int hp)
{
	hp = hp + 1;
}
int main()
{	
	// 지역 변수
	// - 함수 내에 정의된 변수
	// - 정의된 함수의 스택에 저장 됩니다.
	// - 정의된 함수 내에서만 유효한 변수입니다.
	int LocalVariable = 1;

#pragma region 1) 지역변수 예제 
	int hp = 1; 

	cout << "함수 실행전 hp : " << hp << endl;  // 결과 : hp = 1
	IncreseHp(hp);
	cout << "함수 실행후 hp : " << hp << endl;  // 결과 : hp = 1
	// 왜 함수를 통해 hp를 더해 줬는데 출력해보니 값이 같은지?
	// main 함수 안에 정의된 hp 변수를 지역변수라고 하고  IncreaseHp(hp)의 매개변수 hp를 매개변수라고 할 때
	// 매개변수 hp는 지역변수가 직접 들어가는게 아닌 지역변수 hp의 값을 매개변수 hp에 복사 한 것입니다.
	// 그래서 IncreseHp 함수내에서 값을 조작한 것은 함수 내에서만 유효하고 
	// main함수의 지역 변수 hp는 변하지 않는 것입니다.
	// 그러면 어떻게 이문제를 해결해서 함수를 가져다 쓸수 있을까?
	// - 이후 배울 포인터, 참조를 이용해서 해결 할 수 있다고합니다.
#pragma endregion
}