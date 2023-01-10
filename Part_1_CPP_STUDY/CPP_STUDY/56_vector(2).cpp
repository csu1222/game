#include <iostream>
using namespace std;
#include <vector>

// 오늘의 주제 : vector - 2  면접 단골

/*
	컨테이너(Container) : 데이터를 저장하는 객체(자료구조)
	
	컨테이너중 하나인 vector
	vector (동적 배열)
	- vector의 동작원리 (size/capacity)
	- 중간 삽입/삭제
	- 처음/끝 삽입/삭제
	- 임의 접근

	동작원리는 vector(1)에서 알아 봤고 그다음 
	중감 삽입/삭제 , 처음/끝 삽입/삭제 를 알아보려면 개념을 한가지 더 알아야합니다.

	반복자(Iterator) : 포인터와 유사한 개념. 컨테이너의 원소(데이터)를 가리키고, 다음/이전의 원소로 이동가능

	반복자를 실습해보기 위해서 일단 vector를 생성하였습니다. push_back으로 크기를 정하기보다는 그냥 처음부터 10의 크기의
	벡터를 생성해주었고 for문을 돌면서 각각 값을 넣어주었습니다.
	(vector<int>::size_type i = 0; i < v.size(); i++) for문 조건에 이상한 것들이 있는 이유는 우선 i < v.size()로 벡터 v의 
	크기를 하드코딩 해주는것보다 v의 크기를 넣어주어 v의 크기가 달라질때 알아서 변하게끔 하기 위해서입니다.
	그리고 이 v.size()가 unsigned int를 반환하는데 변수 i의 타입을 vector<int>::size_type으로 맞춰주었습니다.
	v는 0 부터 9까지 int를 들고 있는 벡터로 만들었습니다.

	드디어 iterator(반복자)를 만들어 보겠습니다. 
	std::vector<int>::iterator it; 
	이터레이터 it을 만들었습니다. 위와 같이 vector<int>:: 이런 문법이 자주 보일것입니다.

	iterator는 컨테이너마다 자신의 이름으로 한개씩 가지고 있습니다.
	이터레이터는 포인터와 유사하다고 했습니다. 비교를 위해 이터레이터 변수 it과 포인터 변수 ptr을 준비하고
	it = v.begin();
	ptr = v[0];
	으로 벡터v 의 첫번째 값을 가리키도록 하였습니다.
	cout 으로 *if 과 *ptr을 출력 해보면 둘다 v의 첫번째 값을 출력하였습니다.
	
	it과 ptr의 메모리를 까보면, ptr은 알고 있듯이 그냥 주소를 담는 바구니 입니다. 반면 it 이터레이터는 
	ptr처럼 주소를 담고있는 바구니와 또다른 정보를 가지고 있었는데요 이 데이터는 이터레이터가 어떤 컨테이너 소속인지 
	알려주는 정보입니다. 

	포인터 ptr에서 ptr++, ++ptr 을 하면 그냥 ptr에 담겨있는 주소에 1을 더하는게 아닌 ptr이 가리키고 있는 타입의 크기만큼 증가,
	즉, 다음 데이터를 가리키는 것이었습니다.
	이터레이터 it 도 똑같이 it++, ++it 이 다음 데이터를 가리킵니다. 
	++ 뿐만아니라  --it , it += 2, it = it - 2 등과 같이 응용할 수 있습니다.

	그러면 응용으로 
	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();
	를 만들었습니다. 그리고 디버깅으로 들어가 메모리상에서 어떤 값을 가지고 있는지 확인해본 결과
	itBegin은 0 즉 벡테 v의 시작 원소를 정확히 가리키고 있었고 
	itEnd는 -33686918라는 쓰레기 값을 가지고 있었습니다.
	그러면 itEnd의 값이 아닌 주소를 보면 v의 값이 9 다음의 주소를 가리키고 있습니다. 즉, v.end();는 
	벡터의 마지막 값을 얻기 위한게 아닌 벡터의 끝을 판별하기위한 기능입니다.
	
	이 begin, end를 응용하면 
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		cout << (*it) << endl;
	이렇게 내용물을 하나씩 꺼내볼수 있습니다. 
	for (vector<int>::size_type i = 0; i < v.size(); i++)
		v[i] = i;
	의 경우와 뭐가 다른가 할 수 있는데 밑의 경우는 벡터가 배열이기도 하니까 쓸수 있는 방법이고 
	위의 begin, end를 사용한 방법은 다른 컨테이너들에서도 사용할수 있는 방법입니다.

	또한가지 디테일은 위에 코드의 for문 마지막에 ++it 가 있는데 ++it과 it++ 둘다 모든 내용물을 꺼내는것은 같은데
	++it를 쓰고 있는 이유는 이전에 연산자 오버로딩에서 배웠던 내용인데 ++it의 경우 멤버 연산자 함수에서 자신에 1을더하고 
	*this를 바로 리턴하고 it++의 경우 연산전 자신을 임의의 변수에 복사후 자신에게 1을 더하고 임의변수에 복사해놓았던값을 
	리턴합니다. 
	즉, ++it는 연산하고 바로 , it++는 중간에 복사하는과정이 끼어있어서 성능에 약간의 차이가 있다.

	이터레이터로 각 내용물을 꺼내는게 더 복잡해보이는데 꼭 써야할까?
	- iterator는 vector뿐 아니라, 다른 컨테이너에도 공통적으로 있는 개념
	- 다른 컨테이너는 v[i]와 같은 인덱스 접근이 안 될 수도 있음
	

	참고 ) 
	const_iterator, cbegin, cend : const로 데이터를 읽기전용으로 사용할때
	revers_iterator, rbegin, rend : 역방향 반복자로 뒤에서부터 읽어나감


	이제 원래 알아보려던 세가지에 대해

	- 중간 삽입/삭제 (BAD)
	- 처음/끝 삽입/삭제 (BAD/GOOD)
	- 임의 접근 (GOOD)

	vector = 동적 배열 = 동적으로 커지는 배열 = 배열이긴 배열이다.
	vector가 배열이긴 배열이기 때문에 
	원소가 하나의 메모리 블록에 연속하게 저장된다!!

	0 1 2 3 4 
	가 배열의 원소들일때 한 가운데에 5를 삽입하려면 금방생각하면 쉬워보이지만 메모리상으로 자기위치가 정해져있기때문에
	0 1 [ ] 2 3 4 
	로 2 3 4 가 한 칸씩 옯겨서 복사한다음 [ ] 자리에 5를 넣어야 합니다.
	지금이야 데이터가 5개로 작으니 그럴수 있다고 하지만 매우큰 데이터에서 중간에 조금이라도 삽입할 일이 생길때마다 
	그 큰 데이터를 다 복사해야한다면 매우 비효율 적일것입니다.
	
	중간 삭제의 경우도 마찬가지입니다.
	0 1 2 3 4 
	라는 원소들 중 2를 중간 삭제를 하게 되면 
	0 1 [ ] 3 4 
	이렇게 되는데 이대로는 배열의 조건인 원소각 하나의 메모리 블록에 연속하게 저장된다 에 위반합니다.
	연속하게 저장되지않으면 각 위치에 원소가 존재하는지 안하는지 하나 하나 체크해야 하며 그러면 인덱스로 직접 접근이 불가능해집니다.

	처음 삽입/삭제
	처음 삽입/삭제도 마찬가지로 데이터를 추가 삭제 할때마다 데이터 전체가 복사되야 합니다.

	끝 삽입/삭제
	끝에 삽입/삭제하는 경우에는 원래 push_back하듯이 삽입할수 있고 pop_back하듯이 삭제할수도 있습니다. 
	맨끝 메모리만 건드는것이니 앞부분 데이터들은 움직일 필요가 없습니다.

	임의 접근 (Random Access)
	임의 접근이 무엇인가하면 i번째 데이터는 어디 있습니까? 라고 물으면 바로 그 위치의 데이터를 가져올수 있는걸 말합니다.
	바로 배열의 인덱스같을걸 말하는겁니다.
	즉, vector는 임의 접근에 용이한 컨테이너입니다. 왜냐하면 원소들이 하나의 메모리 블럭에 연속하게 저장된다는걸 확신할수 
	있기때문에 각각 의 주소의 내용물을 체크하지 않아도 기준주소에서 몇번째 주소로 직접이동하면 바로 값을 찾을수 있습니다.


	중간 삽입/삭제 가 비효율적이긴 하지만 어쩌다가 사용해야 할 순간이 올지도 모르니 하는법은 알아 둬야 겠습니다.
	vector의 기능중 insert라는 기능이 있는데 첫번째 인자로 어디에 삽입할것인지, 두번째 인자로 무엇을 삽입할 것인지를 넣어주면 되겠습니다.
	ex) vector v = 0,1,2,3,4; 라는 값들이 있는 벡터일때
	v.insert(v.begin() + 2, 5);
	중간 삭제의 경우에는 erase 라는 기능이있습니다. 인자가 하나일떄는 그 위치의 값을 삭제 , 인자가 두개 일때는 
	두 인자사이의 범위의 값들을 삭제
	ex) 
	v.erase(v.begin() + 2);
	v.erase(v.begin() + 2, v.begin() + 4);

	위 insert와 erase들 은 기능도 하지만 반환으로 이터레이터를 반환도 합니다. 각 어떤 이터레이터를 반환하냐 하면
	isert(v.begin() + 2, 5)  = 첫번째 인자로 준 v.begin() + 2 라는 주소를 가진 이터레이터를 반환
	erase(v.begin() + 2) = 역시 인자로 준 v.begin() + 2 의 주소를 가진 이터레이터
	erase(v.begin() + 2, v.begin() + 4) = 이것 역시 첫번째 인자의 주소를 가진 이터레이터를 반환

	다시 erase를 응용해서 벡터 v를 쭉 살펴보고 값이 3인 데이터가 있으면 일괄 삭제 하고 싶다. 라는 예제를 살펴보겠습니다.
	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		int data = *it;
		if (data == 3)
		{
			it = v.erase(it);
		}
		else
			++it;
	}
	맨 윗줄 부터 살펴보면 이터레이터로 컨테이너의 내용물을 살펴보는 for문입니다. 여기서 평소와 다른점은 ++it 가 else문에
	가 있는것입니다. 이건 좀 있다 다시 이야기 하고 
	다음줄로 매 반복마다 이터레이터의 값을 int변수 data에 담아서 data가 3과 같은지 체크하고 있습니다. 여기는 문제없이 이해
	할수 있습니다.
	다음줄은 왜 v.erase(it); 만 하지 않고 이터레이터 변수 it에 다시 반환하고 있을까요? 
	이터레이터는 포인터와 비슷하지만 포인터처럼 주소만 담고 있는것이 아니라 이터레이터 자신이 어디소속인지를 기록해놓은 데이터를 
	하나 더 가지고 있다고 했습니다. 그런 이터레이터를 그냥 erase하게 되면 그 주소의 값뿐만아니라 자기 소속까지 삭제가 됩니다.
	소속이 삭제된상태로 다음 반복을 돌면 이터레이터의 소속이 불분명해서 크래쉬가 납니다.
	erase는 자신이 기능을한 그 주소를 반환한다고 하였습니다. 그래서 erase(it)로 자신과 소속이 다 삭제되는 동시에 
	새로운 이터레이터에 작업을 한 주소를 주어서 다음 반복을 돌수 있게 해주었습니다.
	여기서 다시 erase를 하면 평소라면 다음 주소에 있을 값이 현재 주소로 옮겨지게되므로 그냥 ++it를 해버리면 한 원소를 
	조건체크를 안하고 넘어가는 셈이 됩니다. 그래서 ++it를 for문 괄호 안에 넣지 않고 erase하지 않았으면 다음으로 넘어가는 식으로 
	else ++it; 로 처리 하였습니다.
*/



int main()
{
	vector<int> v(10);

	v.reserve(1000);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
		v[i] = i;

	//vector<int>::iterator it;
	//int* ptr;

	//it = v.begin();
	//ptr = &v[0];

	//cout << (*it) << endl;
	//cout << (*ptr) << endl;

	
	/*it++;
	++it;
	ptr++;
	++ptr;*/

	// 응용
	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();
	
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		cout << (*it) << endl;

	// ptr 버전
	int* ptrBegin = &v[0];  // v.begin()._Ptr;
	int* ptrEnd = ptrBegin + 10;  // v.end()._Ptr;
	for (int* ptr = ptrBegin; ptr != ptrEnd; ++ptr)
		cout << (*ptr) << endl;

	// const int* 와 비슷한 const iterator
	//vector<int>::const_iterator cit1 = v.cbegin();
	//*cit1 = 100;    // const라서 내용물을 수정하는것이 문법적으로 막힘

	// 역방향 반복자
	for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it)
	{
		cout << (*it) << endl;
	}


	//vector<int>::iterator insertIt = v.insert(v.begin() + 2, 5);
	//vector<int>::iterator eraseIt1 = v.erase(v.begin() + 2);
	//vector<int>::iterator eraseIt2 = v.erase(v.begin() + 2, v.begin() + 4);

	// 쭉~ 스캔을 하면서 3이라는 데이터가 있으면 일괄 삭제하고싶다면
	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		int data = *it;
		if (data == 3)
		{
			it = v.erase(it);
		}
		else
			++it;
	}


	return 0;
}