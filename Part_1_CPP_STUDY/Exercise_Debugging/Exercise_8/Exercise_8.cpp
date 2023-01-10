#include <iostream>
using namespace std;
#include "Knight.h"
#include "Archer.h"
#include "Mage.h"
#include "Pet.h"

// [Bug Report #8]
// 펫을 궁수의 생성자에서 만드는 것이
// 깔끔하지 않다는 사장님의 지적에 따라
// 일단 밖으로 빼서 펫을 생성하게 수정했습니다.

// 그런데 막상 밖으로 빼니 또 크래시가 납니다.
// 가만히 있으면 중간은 간다던데...
// 뭐가 문제일까요?

/*
~Archer에서 _pet을 delete를 하였을때 크래시가 났습니다. 그전에 _pet의 값인 pet을 main함수의 지역함수로 생성했었습니다. delete는 new 
문과 함께 사용하는 문법입니다. new로 Heap영역에 데이터를 할당하고 delete로 그 메모리를 해제하는 문법인데 스택 메모리에 있는 데이터인 pet을 delete로 해제하려고 해서 크래쉬가 난것 같습니다.
해결은 main의 pet을 Pet* pet = new Pet(); 로 Heap영역에 생성해야 
하겠습니다.
*/

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	while (true)
	{
		// 나중엔 궁수, 법사 등 추가 예정
		Player* p = nullptr;

		switch (rand() % 3)
		{
			case 0:
			{
				p = new Knight();
				p->_hp = 100;
				p->_attack = 100;
				break;
			}	
			case 1:
			{
				// 여기서 같이 만들어준다
				// 이런 저런 펫 정보 추가될 예정
				Pet* pet = new Pet();

				// Archer를 만들 때 pet 정보도 넘겨준다
				p = new Archer(pet);
				p->_hp = 100;
				p->_attack = 100;

				break;
			}		
			case 2:
			{
				p = new Mage();
				p->_hp = 100;
				p->_attack = 100;
				break;
			}			
		}

		delete p;
	}

}
