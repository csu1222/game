﻿#include <iostream>
using namespace std;


// 이렇게 주석을 달수 있어요
// ctrl + k, c 로 한번에 주석달기 (Comment의 약자)
// ctrl + k, u 로 한번에 주석 풀기  (UnComment의 약자)

/*
주석이 여러줄이라면 이 범위 내에 주석을 달 수 있어요
*/

// 프로그래밍을 단순하게 생각하면 
// 적절한 데이터를 저장하고 가공하는 것
// 즉 데이터 + 로직 이라는 두 파트로 나눌 수 있습니다.

// 그 중에서고 가장 빈번히 사용되는 정수에 대해 알아 봅시다.


// 변수를 하나 만들어 보겠습니다
// 변수 선언 방법:
// [타입] [이름];  혹은
// [타입] [이름] = [초기값];

// 밑과 같이 변수 선언할 때 초기값이 있으면 .data 영역에 할당됩니다.
int  hp = 100;


// 반대로 변수 선언시 값이 없거나 0이면 .bss 영역에 할당됩니다.

// 정수형의 종류   : 각 정수형에서 표현 방법이 변하는게 아니라 가지고 있을 수 있는 데이터 크기가 다릅니다.
// 각 데이터형 앞에 signed [타입] [이름]; 같이 signed가 생략되어 있다고 합니다. 데이터 들의 최상위 비트는 양수 음수 부호일것입니다.
char a;     // 1바이트  어셈블리에서 byte  ( -128 ~ 127 )
short b;    // 2바이트       "       word  ( -32768 ~32767 )
int c;      // 4바이트       "       double word  ( 대략 -21.4억 ~ 21.4억 )
__int64 d;  // 8바이트 (long long 으로 쓰기도 합니다.) 어셈블리에서  quarter word   (경단위가 넘어가는 큰수)
//long long e;


// unsigned 는 최상위 비트를 양수 음수 부호가 아닌 그냥 수로 봐서 양수만 다루겠다는 의미입니다.
unsigned char ua;     // 1바이트  ( 0 ~ 255 )
unsigned short ub;    // 2바이트  ( 0 ~ 655355)
unsigned int uc;      // 4바이트  ( 대략 0 ~ 42억 )
unsigned __int64 ud;  // 8바이트  ( 0 ~ 경단위가 넘어가는 큰수)

// 정수 타입을 그냥 int 4바이트를 쓰면 안되나요?
// -> 콘솔, 모바일 게임 -> 늘 메모리가 부족합니다.
// -> 온라인 게임 -> 항상 __int64로 쓴다고하면 원래 필요한 용량 * 동시 접속자 수 만큼의 손해를 보게 됩니다.

// 참고) 이론적으로 양수만 사용할 데이터(레벨 등)은 unsigned만 사용하는가?
// 무조건 unsigned 만 사용하는지 의견이 갈립니다.
// - 레벨이 음수라는 것은 말이 안된다 -> 그럼 차라리 그 자리에서 프로그램 크래시를 내서 버그를 잡자
// - unsigned / signed 사이의 변환에 가끔 버그가 일어날 수도 있다 
// 팀마다 의견이 갈릴 수 있음 무엇이 무조건 옳은지는 알 수 없습니다.


int main()
{   
    // 정수 범위를 오인해 일어날 수 있는 버그
    // 정수 오버 플로우
    b = 32767;  // 위에서 short 정수 타입 변수였습니다.
    b = b + 1;
    cout << b << endl;    // 결과 -32768 출력 왜냐하면 0111 1111 이 32767 인데 여기에 1을 더하면 1000 0000 이고 이건 -32768입니다.


    // 정수 언더 플로우
    ub = 0;
    ub = ub - 1;
    cout << ub << endl;

    cout << "체력이 " << hp << "남았습니다." << endl;
}

