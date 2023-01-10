#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
// 오늘의 주제 : deque

/*
	시퀀스 컨테이너 (Sequence Container)
	데이터가 순서대로 나열되는 형태의 컨테이너
	vector, list, deque

	vector : 동적배열
	list : 이중 연결 리스트
	
	deque : double-ended queue 데크
		일단 벡터처럼 배열의 형태로 데이터를 저장하다가 꽉차면 또다른 메모리에 다시 배열을 만들어서 이어 저장합니다.
		다른 배열을 만들때 벡터처럼 이전 값들을 모두 복사해가는것이 아니라 그냥 다음 값들을 저장합니다. 각 배열은 
		리스트처럼 포인터로 가리킵니다.

		push_back과 push_front를 살펴보면 
		push_back으로 처음 만든 배열이 꽉차게 되면 기존 배열은 두고 다른 메모리에 새로 배열을 만들어 이어지는 값을 저장합니다.
		
		push_front로 앞에 값을 넣어주면 기존 배열의 앞쪽으로 이어지는 배열을 만들고 거기에 push_front한 값을 저장, 인덱스를0으로
		설정합니다.

	- deque 동작원리
	- 중간 삽입/삭제 (BAD / BAD)
	- 처음/끝 삽입/삭제 (GOOD / GOOD)
	- 임의 접근  (GOOD)


	임의접근 : deque는 [ ] 연산자로 값에 접근이 가능했습니다. vector처럼 완전히 연속된 메모리 블럭이 아닌데 어떻게 가능한지 
		알아보면, deque를 f12로 원본 코드에 접근해서 operator[] 를 찾았습니다. [] 연산자는 
		return *(_Unchecked_begin() + static_cast<difference_type>(인자));
		로 구현이 되어있었습니다. 앞의 _Unchecked_begin()이 이터레이터고 이 이터레이터에 인자만큼 +연산을해서 그곳의 *(역참조)
		를 반환하는 것 같습니다.
		그러면 deque의 이터레이터를 다시 알아봐야겠습니다.

		deque의 이터레이터 코드에서 operator* 의 구현부를 보면
		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % _Block_size;
		return _Mycont->_Map[_Block][_Off];

		추측으로 각각의 배열들을 _Block으로 부르고 블록마다 번호가 있고 , 블록의 크기를 _Block_size라고 하는것 같습니다.
		_Myoff는 구하고자 하는 값이 몇번째 위치에 있는지 이고, _Off 변수는 _Myoff가 블록내에 인덱스가 뭔지 해서  
		_Getblock으로 몇번째 블록인지 얻고 
		_Mycont->_Map[_Block][_Off]; 로 아직 잘 모르겠지만 _Map이라는 기능으로 몇번째 블록에 몇번 인덱스의 값을 반환하는
		연산자 오버로딩 같습니다.

		이런 방법으로 임의 접근이 용이한 deque는 각 블록의 메모리가 비어있지 않다고 확신 할수 있어야 접근이 가능해집니다. 
		그런 이유로 중간 삽입 삭제가 어려운 것입니다. 

		그리고 deque는 double_ended queue, 앞뒤로 삽입 삭제가 가능한 queue라는 의미인데 
		이 queue는 다음에 배우게 될 것입니다.
	 
*/


int main()
{
	//deque<int> dq;

	//dq.push_back(1);
	//dq.push_front(2);

	//cout << dq[0] << endl;

	vector<int> v(3, 1);
	deque<int> dq(3, 1);

	v.push_back(2);
	v.push_back(2);

	dq.push_back(2);
	dq.push_back(2);

	dq.push_front(3);
	dq.push_front(3);

	deque<int>::iterator it;

	return 0;
}