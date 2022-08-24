#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
using namespace std;

// 오늘의 주제 : set, multimap, multiset

/*
	map 이외의 연관 컨테이너들
	set, multimap, multiset

	set : set과 map의 차이점은 map은 <key, value> 두 값을 저장하는 컨테이너였지만
		set은 key만 저장하는 컨테이너라고 할 수 있습니다. key이지만 key값을 곧 value로 사용하는 것입니다.

		set의 사용법은 map과 거의 유사합니다.
		넣고	(insert)
		빼고	(erase)
		찾고	(find)
		반복자	(set<int>::iterator) (*it)

		map과 조금 다른점이 있다면 []연산자를 지원하지 않는다는것 []는 key와 value값이 따로 있는 map이라서 지원하는것이었고
		key = value인 set는 []연산자가 필요하지 않습니다.


	multimap, multiset : 이 둘은 이미 배운 map과 set에서 중복 key를 허용하는 컨테이너입니다. 
		key, value의 형태를 사용하는것 자체가 고유 key로 value들을 찾기 쉽게 하기위해서 사용하는 것이라 
		중복 key를 허용하는 multi 시리즈 들을 사용하는건 잘 없는 일이긴 하지만 일단 어떤 개념인지 배워보겠습니다.

	multimap : 대부분 map과 유사하고 다른점은 key가 중복허용이 된다는 것입니다. 그러다보니 erase로 어떤 key를 지울때 
		여러 노드가 한번에 지워질 수도 있고 그중 하나의 노드만 골라 지우고 싶다면 find로 같은 key값을 지닌 노드중 맨 처음
		노드를 가져다가 ++ -- 같은 연산자로 지울 노드를 골라 erase를 할 수 있습니다. 
		find로 하나만 고르는게 아닌 같은 key를 가진 노드들을 전부 고를때는 equal_range로 해당 범위의 이터레이터를 pair로 
		반환할수 있습니다. pair가 아니라 각 이터레이터를 원하면 lower_bound, upper_bound로 이터레이터를 얻을수 있습니다.

	multiset : multimap과 거의 똑같습니다. key값만 가지는 multimap이라고 생각하면 되겠습니다.
*/
#include <set>

int main()
{
	// set
	// key = value
	set<int> s;
	s.insert(10);			// 넣고
	s.insert(30);			
	s.insert(60);			
	s.insert(90);			
	s.insert(20);			
	s.insert(40);

	set<int>::iterator findIt = s.find(10);				// 찾고
	
	s.erase(10);			// 빼고
	
	set<int>::iterator it;	// 반복자
	for (set<int>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << *it << endl;
	}

	cout << "--------------------------------------------------" << endl;

	// multimap
	// 중복 key를 허용하는 map

	multimap<int, int> mm;

	mm.insert(make_pair(1, 100));		// 넣고
	mm.insert(make_pair(1, 200));		
	mm.insert(make_pair(1, 300));		
	mm.insert(make_pair(2, 400));		
	mm.insert(make_pair(2, 500));

	multimap<int,int>::iterator findIt2 =  mm.find(1);							// 찾고	
	// find는 key에 해당하는 첫 iterator를 반환합니다.
	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> itPair;
	itPair = mm.equal_range(1);
	// equal_range는 key에 해당하는 iterator들의 첫 iterator를 pair의 first로 ,
	// 마지막 iterator의 다음 iterator를 second로 줍니다.
	for (multimap<int, int>::iterator it = itPair.first; it != itPair.second; ++it)
	{
		cout << it->first << " " << it->second << endl;
	}
	// equal_range가 반환하는 pair를 한번에 추출하는거 말고 따로 추출하고 싶다면
	multimap<int, int>::iterator lower = mm.lower_bound(1);	// equal_range 의 pair에 first에 해당
	multimap<int, int>::iterator upper = mm.upper_bound(1);	// equal_range 의 pair에 second에 해당

	unsigned int count;
	count = mm.erase(1);					// 빼고
	// erase는 key에 해당하는 모든 노드를 삭제하고 삭제한 카운트를 unsigned int로 반환합니다.
	cout << "erase count " << count << endl;

	// multimap<int, int>::iterator;		// 반복자
	// 모든 데이터 순회
	for (multimap<int, int>::iterator it = mm.begin(); it != mm.end(); ++it)
	{
		cout << it->first << " " << it->second << endl;
	}


	cout << "--------------------------------------------------" << endl;

	multiset<int> ms;

	// 넣고
	ms.insert(100);
	ms.insert(100);
	ms.insert(100);
	ms.insert(200);
	ms.insert(200);

	// 찾고		
	multiset<int>::iterator msFindIt = ms.find(100);

	pair<multiset<int>::iterator, multiset<int>::iterator> msItPair;

	msItPair = ms.equal_range(100);
	multiset<int>::iterator mslower = ms.lower_bound(100);
	multiset<int>::iterator msupper = ms.upper_bound(100);

	for (multiset<int>::iterator it = msItPair.first; it != msItPair.second; ++it)
	{
		cout << *it << endl;
	}

	for (multiset<int>::iterator it = mslower; it != msupper; ++it)
	{
		cout << *it << endl;
	}



	return 0;
}