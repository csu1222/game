#include <iostream>
using namespace std;

// 오늘의 주제 : 포인터 vs 참조

struct StatInfo
{
	int hp;			// +0
	int attack;		// +4
	int defence;	// +8
};

// [매개변수][RET][지역변수(info)] [매개변수(&info)][RET][지역변수]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 15;
	info->defence = 5;
}

StatInfo globalInfo;

void PrintInfo(StatInfo* info)
{
	if (info == nullptr)  // nullptr 예외 처리
		return;

	cout << "-------주소 전달 방식 -------" << endl;
	cout << "HP : " << info->hp << endl;
	cout << "ATT : " << info->attack << endl;
	cout << "DEF : " << info->defence << endl;
	cout << "---------------------------" << endl;

	// 별 뒤에 붙힌다면?
	// - 매개변수로 (StatInfo* const info)
	// info라는 바구니의 내용물(주소)를 바꿀 수 없음 (주소값 고정!)
	// 별 앞에 붙힌다면?
	// - 매개변수로 (const StatInfo* info) or (StatInfo const* info)
	// info가 '가리키고' 있는 바구니의 내용물을 바꿀 수 없다.
	// '원격' 바구니의 내용물을 바꿀 수 없다.

	// 두 const를 둘 다 사용 할 수도 있다 (const StatInfo* const info)

	// info[ 주소값 ] 을 조작하는 것과
	//info = &globalInfo;
	// 주소값[ 데이터 ] 를 조작하는것
	//info->hp = 1000;
}


void PrintInfo(const StatInfo& info)
{
	cout << "-------참조 전달 방식 -------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.attack << endl;
	cout << "DEF : " << info.defence << endl;
	cout << "---------------------------" << endl;

	// 매개변수에 const를 붙힘으로써 누군가 원본을 조작하는 실수를 줄임
	// info.hp = 1000; (에러)
}

// nullptr의 용도 
// 특정 조건을 만족하는 몬스터를 찾는 함수
StatInfo* FindMonster()
{
	// TODO : Heap 영역에서 뭔가를 찾아봄
	// 찾았다!
	// return monster~;

	// 못찾았다면?
	return nullptr;   
 }


// OUT 으로 참조에 표시법
#define OUT		// OUT에 다른 의미를 부여하지 않고 그냥 명시만해서 가독성 의미로 씁니다.
void ChangeInfo(OUT StatInfo& info)
{
	info.hp = 1000;
}

int main()
{
	// 포인터 vs 참조 세기의 대결!
	// 성능 : 똑같다!
	// 편의성 : 참조 승!

	// 1) 편의성 관련 
	// 편의성이 좋다는게 꼭 좋은 것만은 아니다.
	// 포인터는 주소를 넘기니 확실하게 원본을 넘긴다는 힌트를 줄 수 있는데.
	// 참조는 자연스럽게 모르고 지나칠 수 있음! (항상 함수에 참조로 전달한다고 표시가 되어 있는게 아님!)

	// 함수이름에 정보가 부족할 경우 매개변수로 &info가 오니 이건 포인터로 원본을 건드리는 구나 
	// 바로 알 수 있습니다.

	// 참조를 사용할 경우 함수이름과 매개변수의 상태에서 이 함수가 참조로 원본을 건드리는 것인지 알 수 없습니다.
	// 가뜩이나 프린트 라고 보이는 함수는 원본을 건드릴 필요가 없어서 더욱 햇갈립니다.
	// 이 코드가 원본을 훼손할 가능성이 있다는걸 알아차리기 힘듬
	// but 그래도 마음대로 고쳐지는걸 막는 방법은 있습니다.
	// - const를 사용해 변경되지 않게 한다던가 
	
	// 참고로 포인터에도 const 사용 가능
	// * 기준으로 앞에 붙히느냐, 뒤에 붙히느냐에 따라 의미가 달라진다.


	// 2) 초기화 관련 
	// 참조 타입은 바구니의 2번째 이름을 지어주는 느낌(별칭?)
	// -> 반대로 생각하면 참조하는 대상이 없으면 안됨.
	// 반면 포인터는 그냥 어떤 주소라는 의미
	// -> 대상이 실존하지 않을 수도 있음.
	// 
	// 포인터에서 '없다' 는 의미로? nullptr.   but 반대로 포인터 타입은 항상 nullptr 타입이 들어올수 있기 때문에 
	// if-else로 체크해서 예외 처리를 해야 할 것입니다. (25 라인)
	// 참조 타입은 이런 nullptr이 없으므로 60라인에 있는 nullptr의 활용 같은걸 못하는 단점도 있다~
	// 
	// 그래서 결론은?
	// 사실 team by ream... 정해진 답은 없다
	// ex) 구글에서 만든 오픈소스를 보면 거의 무조건 포인터 사용
	// ex) 언리얼 엔진에선 reference도  애용
	// 
	// Rookiss 선호 스타일)
	// - 없는 경우도 고려해야 한다면 pointer (null 체크 필수)
	// - 바뀌지 않고 읽는 용도 (readonly)만 사용하면 const ref
	// - 그 외 일반적으로 ref 선호 (면시적으로 호출할 때 OUT을 붙인다.)  (77 라인)
	// -- 단 다른사람이 pointer를 사용한 상황이면 pointer를 사용 (섞어 사용하진 않는다)
	// 
	// ex) 
	StatInfo example;
	StatInfo* pointer2 = nullptr;  // or = NULL; or = 0;  : 값이 '없다' 라는 의미 어떠한 주소도 가리키고 있지 않다라는 의미
	StatInfo* pointer;	// 포인터는 대상없이 선언 가능
	pointer = &example;
	PrintInfo(pointer);

	StatInfo& reference = example;  // 참조는 선언때 꼭 대상이 있어야 가능
	PrintInfo(reference);
	// 

	ChangeInfo(OUT example); // 77 라인


	StatInfo info;
	PrintInfo(&info);
	PrintInfo(info);


	// Bonus) 포인터로 사용하던걸 참조로 넘겨주려면?
	// int item = 10;
	// int* ptr = &item;
	// funcByRef(*ptr);    매개변수로 참조값을 받는 함수에 포인터를 쓰려면 포인터 앞에 *을 붙혀 참조 타입으로 변환합니다.
	// 
	// Bonus) 참조로 사용하던걸 포인터로 넘겨주려면?
	// 이경우 반대로 
	// funcByPtr(&ref);  
	//

	return 0;
}