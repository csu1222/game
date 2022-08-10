#include <iostream>
using namespace std;

// 오늘의 주제 : 디버깅 

/*
- F10 (프로시저 단위 실행) : 중단점에 커서를 진행시키는데 프로시저(함수)단위로 진행합니다. 함수내부까지는 안들어 가고 스킵
- F11 (한 단계 씩 실행) : 중단점에 있는 커서부터 한 프로세스씩 실행합니다. 함수나 객체를 만나면 그 내부까지 들어가서 진행합니다.
- F5 (디버깅 시작) : 말그대로 디버깅 시작, 코드진행이 멈출때 까지 쭉 실행하고(중단점을 만나거나 프로그램에서 입력을 받기위해 멈춘경우)
	멈춘 후 부터 여러 디버깅 도구를 이용해 디버깅 할 수 있습니다.
- Shift + F5 : 디버깅 중지 
- 조사식 : 디버깅 상황에서 어떤 변수나 메모리의 값을 보는 기능 (값들을 조사식에서 연산도 가능합니다.) 
	심지어 조사식에서 그 데이터의 값을 수정해볼 수도 있습니다.(임시적으로)
- 호출 스택 : 메인 함수에서부터 여러개의 함수가 곂쳐서 동작 할 경우 호출 스택 도구가 여태껏 타고들어온 함수들을 전부 알려줍니다. 
	더블클릭으로 그 함수로 이동 할 수도 있습니다. 또 중요한 역할이 프로그램이 어느곳에서 크래쉬가 날 경우 크래쉬 난 곳을 덤프라고 하는데
	그때도 호출스택으로 어떤 코드가 문제였는지 확인하는데 도움을 줄 것입니다.
- 중단점 : 디버깅 실행시 이곳에서 코드 진행을 멈춥니다.
- 중단점 도구 : 내가 여태껏 걸어놓은 중단점들을 확인, 관리할 수 있습니다.
- 중단점(조건 걸기) : 이후에 MMORPG를 만든다고 했을때 어떤 특정아이디를 가진 몬스터 하나가 문제여서 그 몬스터에게 딱 중단점을 걸어야 하는데
	일일히 확인해가면서 중단점 걸기가 힘들 것입니다. 그럴때 중단점을 우클릭 후 조건메뉴(혹은 Alt + F9, C)로 조건을 걸 수 있고 조건이 해당
	할 경우에만 중단되게끔 합니다.
- 중단점 (작업) : 코드를 진행하면서 중단점으로 멈추지는 않고 어떤 로그만 출력하고 싶다면 우선 중단점을 만들고 우클리 후 작업을 만들면 따로
	소스코드에 직접 적지 않고 로그를 출력할 수 있습니다.
- 중단점 (커서 옮기기) : 만약 어떤중단점에 도달하기까지 여러 과정을 거치고 여러 조건을 만족해야 올 수 있다고 할때, 이 중단점을 다시 실행
	하기 귀찮을때가 있을 것입니다. 그러면 디버깅중 중단점에는 빨간 동그라미위에 화살표가 있는데 이 화살표를 드래그 드롭으로 다시 실행할 코
	드 위치로 옮기면 그 위치부터 코드가 다시 실행 됩니다. 다시 실행시 이전 시행과 조건이 달라져 본 의도대로 실행되지 않을수 있습니다.

- 한솔루션에서 여러 프로젝트 열기 : 솔루션 탐색기의 최상위 솔루션 항목을 우클릭해 추가 새 프로젝트로 한번에 여러 프로젝트를 만들 수 있습니다.
	사용하려는 프로젝트를 우클릭해서 시작 프로젝트로 설정을 하면서 각 프로젝트를 실행 할 수 있습니다.
- 한번에 여러 프로젝트 실행하기 : 최상위 솔루션 항목을 우클릭후 -> 속성 -> 시작프로젝트 속성 -> 여러개의 시작 프로젝트 설정으로 한번에 여러
	프로젝트를 실행할 수 있습니다.
- 프로젝트 삭제하기 : 삭제할 프로젝트를 선택하고 delete키로 삭제합니다. 그러나 컴퓨터 메모리에서 완전히 삭제된것은 아니고 솔루션 탐색기에서만 
	삭제된 것입니다. 직접 디렉토리로 가서 삭제해주어야 합니다.
*/

/*
디버깅 연습문제 해설 
1. 문제 : 체력과 공격력을 int로 가진 클래스의 기본생성자와 int를 받는 생성자 가있는데 int를 받는 생성자로 객체를 생성하니 공격력 속성이 의도하지않은 값이었다 
	
	solution) int를 받는 생성자에서 공격력값을 초기화해 주지않은 문제였습니다. 공격력 값이 쓰레기 값으로 되어있었습니다. 단순히 초기화 해주었습니다.

2. 문제 : 클래스 포인터 배열의 크기를 10으로 생성했습니다. 이후 반복문으로 각 포인터에 객체를 생성해주었습니다. 그리고 다시 반복문으로 객체의 정보를
	출력하고 delete해주는 코드를 만들었는데 크래쉬가 났습니다.
	
	solution) 이 프로그램을 실행해보면 마지막 객체의 체력이 프린트되지 않고 종료되었습니다. 다음으로 중단점 없이 디버깅을 해보니 예외가 발생하였고
	읽기 엑세스 위반이라고합니다. 
	이럴경우 호출스택을 살펴보는것이 좋다. 호출스택을 살펴본 결과 main에서 PrintInfo를 호출하고 여기서 뻗었습니다. 그래서 PrintInfo를 살펴보니 
	객체의 _hp, _attack을 읽으려다가 크래시가 났습니다. 이러면 에초에 메모리가 유효하지 않은게 문제라고 합니다. 
	우선 문제가 생긴 반복문에 중단점을 걸고 쭉 따라가 보았습니다. 그랬더니 잘 실행되다가 마지막에 크래쉬가 나는걸 확인했습니다.그러면 혹시 
	이 코드가 잘못된 영역을 건드리고 있는게 아닐까 생각해봐야겠습니다. 크래쉬의 이유는 반복문의 조건에 오타가 나서였습니다. int i = 0; i <=
	count; i++ 에서 <= 이 아니라 < 로 고쳐주었습니다. 간단한 문제였습니다만 문제가 PrintInfo구현부에 있을 거라고 짐작하고 그부분만 보고 있었으면
	못찾을 버그 였습니다. 그래서 호출스택을 타고 올라가면서 버그를 찾아야 겠습니다.

3. 문제 : 데미지 피격 실험을 위해 기사1 (체력: 100 / 공격력 :10)과 기사 2 (체력 : 2000 / 공격력 : 200)을 생성했고 기사 2가 기사 1을 공격하여 처치 
	했다는 로그가 나와야하는데 죽었다는 로그가 나오지 않습니다.

	solution) 기사2 가 기사1을 죽이기 충분한 공격력을 가지고 있으니 한방에 죽어야 정상일 것입니다. 그러면 두 부분을 의심해볼 수 있습니다. 
	첫째로, 데미지를 계산하는 로직. 두번째로, 죽음을 판별하는 로직. 먼저 데미지를 계산하는 로직을 보면 int형 damage라는 변수에 공격자의 _attack
	값을 넣어서 피격자의 멤버함수 AddHp(-damage)로 처리하였습니다. 혹시 damage를 음수로 변환해서 더하는 부분이 문제인지 중단점을 걸고 살펴보니 
	제대로 기사1의 체력이 -100으로 피격되었습니다. 그런데 다음 죽음판별 로직에서 살았다고 나옵니다. 죽음 판별 방식이 체력이 0인지를 체크하고 
	bool타입으로 반환하는 방식이었습니다. 체력이 음수까지 내려가는 상황이었으니 체력 <= 0 으로 음수까지 커버하는 방식이었던지 데미지 체크 후 
	체력이 0보다 적으면 0으로 보정해주는 방식이었으면 제대로 실행되었겠습니다.

4. 문제 : 생명력구슬을 도입해서 이 구슬을 먹으면 랜덤으로 체력을 올려주도록 하였습니다. 이 기능이 잘 동작하는지 확신하기 위해 체력 100만을 100만번
	채우는 테스트를 했습니다. 하지만 어찌된 일인지 기사가 죽었습니다.

	solution) int 타입 변수는 값의 범위가 2000000000~ -2000000000 어쩌구 근처 사이 였던걸로 기억하고 있습니다. 지금 테스트에서 더해주는 값이 너무
	크다보니깐 2000000000을 넘어서 최상위 비트열이 올라가면 음수로 바뀌는 현상이 일어난것 같습니다. 그래서 플레이어 클레스에서 멤버 변수로 
	maxHp를 설정해주고 AddHp함수내에서 hp에 value를 더한 값이 maxHp를 넘어가면 maxHp로 보정해주는 기능을 넣어서 해결 했습니다.
	Tip - 체력이 더해지는 과정을 추적해가면서 확인할때 반복문에 중단점을 걸고 조사식에 _hp를 넣어서 확인해가면서 시행하면 좋을것 같습니다. 
	혹은, 반복문의 중단점에 조건을 걸어서 _hp < 0일때 멈추게 하고 그때의 시행횟수에서 1 뺀 시행으로 가서 확인 해보는것도 있습니다.

5. 문제 : 최대 체력개념이 추가되고, 아울러 짜릿한 역전의 희망을 주기위해 체력이 50% 이하로 떨어지면 데미지를 2배로 적용하였습니다.
	그리고 테스트를 위해 기사 두마리를 생성하고 서로 공격을 주고 받습니다. 이때 다시 크래시가 났습니다.

	solution) 중단점없이 디버깅을 실행하니 바로 오류메세지가 뜹니다. 하단에 적혀있는것이 integer division by zero. 정수를 0으로 나누었다고 하네요
	공격데미지를 계산하는 함수 GetAttackDamage 함수로 가봐야 겠습니다. 현재 체력 퍼센테이지를 계산하기위해 int ratio = _maxHp / _hp;로 구하고 
	있습니다. 정수를 0으로 나누었다는것은 분모인 _hp가 0이되었다는 것입니다. 위 기사둘이 치고 받는 테스트에서 우연히 체력도 100, 공격력도 100
	이었으니 한번 공격받게 되면 현제 hp가 0이 되어  오류가 난것 같습니다. 해결은 왠만해선 0이 되지않을 maxHp를 분모로 두면 될것 같습니다.
	int percentage = _hp / _masHp * 100  으로 백분율로 나타냈습니다. 하지만 여기서 간과할 만한 점이 만약 hp가 46정도 일때 계산한다고 했을때 
	이상하게도 percentage가 42가 아니라 0이 됩니다. 그이유는 _hp, _maxHp 둘다 정수형 데이터이기 때문에 둘인 나누면 0.46이 나오지만 소숫점 아래로는 
	버리기 때문에 0이고 여기에 100을 곱해봤자 0이되는 것입니다. 그래서 분자인 _hp에 미리 100을 곱해주고 최대체력으로 나눠주면 
	4600 / 100 이기 때문에 소숫점 아래로 내려가지 않고 46퍼센트라는 값을 구할 수 있습니다.

6. 문제 : 반격 시스템이 도입되어서 기사는 자신을 공격한 대상을 똑같이 반격합니다. 기사2를 생성해서 위 사양을 테스트하는데 크래시가 났습니다.

	solution) 어떤 에러가 났는지 디버깅을 해보면 약간의 시간이 지나고 에러 메세지가 뜹니다. Stack Overflow라고 합니다. 딱히 스택에 뭘 하진 않은거 
	같은데 왜그럴까 호출 스택을 보니 심상치 않게 많은 함수들이 호출되었습니다. 위 사양에서 기사는 공격을 받으면 반격을 한다고 했는데 이 말은
	우선 데미지를 받고 나서 다시 인자로 받은 상대방의 OnDamaged를 호출하게됩니다. 그러면 다시 자신도 공격을 받고 반격하고 상대도 공격받고 반격하고
	를 반복하면서 재귀적으로 자신을 호출하게 됩니다. 그러면 무한루프를 돌아야지 왜 크래쉬가 나느냐? while(true)같은 무한 루프는 한번의 루프마다 
	자신안의 데이터는 리셋이 되고 다시 루프를돕니다. while외부의 변수를 조작할때는 그 값이 유지되지만 루프 안의 데이터는 사라집니다. 반면 
	재귀적으로 자신을 부르는 재귀함수는 한번 루프를돌고 자신의 주소와 매개변수, 지역변수를 스택에 남기고다음 루프에 들어갑니다 이후 루프도 받은 
	매개변수 , 자신의 bp 베이스포인터, 지역 변수를 남기고 다시 루프를 돕니다. 이렇게 재귀함수는 루프마다 자신의 데이터를 삭제하지 않고 루프를 
	도는 겁니다.그래서 스택 메모리가 넘치게되면 스택오버플로우가 일어납니다. 해결은 반격자체가 한공격에 한번만 일어나게하던가 아니면 
	재귀를 빠져나갈 조건을 만들어야 겠습니다. 요구 사양이 반격이 계속되게끔하는걸 지도 모르니 재귀를 빠져나갈 조건으로 해결했습니다. 더 안전하게
	처리하려면 damage가 0일경우 다시 무한히 재귀할 상황까지 처리해야겠습니다.

7. 문제 : 이번엔 기사 말고도 궁수,법사라는 직업이 추가되었고 궁수에는 펫이 있어야 겠다 해서 궁수는 펫 개념이 있습니다. 펫의 기능은 아직 없고 
	궁수 생성자 내에서 펫을 생성해주고 있습니다. 메인 함수에서 직업들을 랜덤으로 생성하고 delete해주는 무한루프로 테스트를 하고 있습니다. 분명
	생성한 객체들을 루프 마지막에 delete해주고 있는데 메모리가 계속 증가하고 있습니다. 이건 메모리 누수 현상인데 어찌된 일일까요?

	solution) 디버깅을 해보면 메모리를 다썻는데 펫객체를 만들다가 메모리가 부족하다고 합니다. 즉, 펫을 생성하는게 메모리 누수의 원인입니다.
	지금껏 배운내용중에 이것과 관련된 내용이 있었습니다. 바로 소멸자와 관련된 내용입니다. 부모 클래스와 자식 클래스가 있을때 자식클래스를 부모
	클래스의 포인터로 생성할 수 있었습니다. 생성해서 사용후 소멸할때 부모클래스 포인터인 그대로 소멸하면 부모의 소멸자만 호출하고 끝났고 자식
	클래스에만 있던 데이터는 소멸하지 않았습니다. 깔끔히 해제해주려면 소멸자를 호출하기전에 이 객체가 어떤 자식 클래스인지 확인하고 그 자식 
	클래스로 타입변환을 시켜준 후 자식 클래스의 소멸자부터 호출해서 차근차근 메모리를 해제하는 방법이 있었고 또 다른 방법으로 애초에 부모 클래스
	의 소멸자를 가상 소멸자 함수로 만들면 vftable (가상 함수 테이블)에 이 객체의 원본이 어디인지 기록해 자동으로 자신의 소멸자부터 호출하였습니다.

8. 문제 : 펫을 궁수의 생성자에서 만드는것이 깔끔하지 않다는 의견으로 펫을 밖으로 빼서 생성했습니다. 하지만 막상 테스트를 하니 다시 크래시가	
	났습니다. 

	solution) 문제점은 펫을 밖에서 생성할 때 힙영역에 만든것이 아니라 스택영역에 만들었다는 것 입니다. 그리고 또 한가지 생성한 지역변수가 유효한
	범위는 궁수를 생성하는 { }의 안입니다. 궁수가 생성이 된뒤 바로 펫이 있는 스택메모리는 유효하지 않게 됩니다. 이후 객체를 delete를 하면서 
	궁수의 소멸자에서 펫을 delete 해주는데 이 delete는 new와 한 세트로 쓴다고 했습니다. 힙영역의 메모리를 해제하는 delete로 스택영역 게다가 
	유효하지도 않은 메모리를 해제하려고 해서 생긴 오류입니다. 해결은 펫을 생성할때 동적할당으로 생성후 그 포인터를 인자로 넘겨주어야 겠습니다.
	
*/

void Test2()
{
	int b = 0;
}

void Test1()
{
	int a = 0;
	
	Test2();
}


int main()
{
	int number = 10;

	Test1();

	return 0;
}