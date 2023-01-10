#include <iostream>
using namespace std;
#include "Knight.h"

// [Bug Report #6]
// 반격 시스템이 도입되어,
// 기사는 자신을 공격한 대상을 똑같이 공격합니다.
// 기사 2을 생성해서 위 사양을 테스트하는데...
// 바로 크래시가 나고 있습니다.
// 또 뭐가 문제인지 원인을 찾아주세요!

/*
크래시가 난 이유가 스택 오버플로우입니다. 즉 함수가 너무 많이 실행되서 스택메모리가 넘쳤다는 것입니다.
그 이유는 OnDamaged함수에서 자신이 피격을 당한후 자신의 체력을 체크해서 0이 되었으면 사망처리후 리턴해야하는데 사망처리없이 서로 체력이0인상태로 계속 공격을 주고받아서 그렇습니다.
*/

int main()
{
	Knight* k1 = new Knight();
	k1->_hp = 100;
	k1->_maxHp = 100;
	k1->_attack = 100;

	Knight* k2 = new Knight();
	k2->_hp = 100;
	k2->_maxHp = 100;
	k2->_attack = 100;

	k1->OnDamaged(k2);

	delete k1;
	delete k2;
}
