#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>


// 오늘의 주제 : delete ( 삭제된 함수 )

/*
	이번에 배울 delete는 동적할당 시간에 배운 new-delete문법이 아닌 삭제된 함수를 만드는 문법입니다.
	nullptr을 구현할때 잠깐나왔었는데 그때는 private로 operator&을 막아놨습니다. 그런식으로 사용하는 문법입니다.
	하지만 private로 막는 문법은 외부에서는 접근할 수 없지만 자기 내부에서는 접근할 수 있는 반쪽짜리 문법이었습니다.

	밑에 예시로 만든 Knight 클래스에서 대입연산자(=)를 막기위해서 private로 구현하였습니다. 
	그리고 실용적이진 않지만 문법적인 학습을 위해 Admin이라는 클래스에서 멤버 함수로 Knight를 복사연산자 (=)를 사용해봤습니다.
	
	Knight클래스에서 private로 막아 놨으니 Admin에서 복사연산자를 사용할수 없었지만 
	여기서 또 새로운 문법인 friend 문법으로 frined class Admin을 Knight 클래스 내부에 입력하면 다른 클래스도 private영역에 접근할수 있게됩니다.
	이렇게 사용 못하게 막으려던 = 을 사용하게 되어 버렸습니다.

	이 예시는 잘사용하지 않는 상황(friend class까지 사용해서 접근하는 상황)을 예시로 들었지만 그래도 결국 뚫리긴 한다는 걸 알려줍니다.
	private라도 구현을 한 이상 접근을 할수 있습니다.

	그래서 Knight클래스의 void operator= (const Knight& k); 이렇게 { } 로 구현부를 만들지 않으면 빌드는 통과가 되지만 실행하면 링크에러가 납니다.
	이러면 분명 = 연산자의 사용을 막을수는 있지만 빌드는 통과되서 한눈에 알아보기 힘듭니다.

	이런 문제를 delete 문법으로 해결 할 수 있게됩니다. 
	delete문법의 사용법은 간단합니다. 위의 예시를 다시 들면
	public:
		void operator= (const Knight& k) = delete;
	이렇게 함수 선언후 = delete; 를 해주면 됩니다. 
	delete를 사용하면 빌드전 컴파일러가 안된다고 미리 알려주기도하고 에러 메세지로 삭제된 함수를 참조하려고 한다고 친절히 알려줍니다.


*/


class Knight
{
	// 만약 Knight = Knight 같은 대입 연산자를 막고싶다면
//private:
//	void operator= (const Knight& k);
//		
//	// private에 접근 할수 있도록 열어주는 문법
//	friend class Admin;

	// delete 문법으로 막기
public:	// delete는 public으로 열어놓는게 관례라고 합니다.
	void operator= (const Knight& k) = delete;
private:
	int _hp = 100;
};

// 실용성은 없지만 문법 연습용 : private로 사용 막는것의 불완전성
class Admin
{
public:
	void CopyKnight(const Knight& k)
	{
		Knight k1;

		k1 = k;
	}
};


int main()
{
	Knight k1;

	Knight k2;

	//k1 = k2;

	Admin a1;
	a1.CopyKnight(k1);

	return 0;
}