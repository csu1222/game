#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

// Stack (LIFO Last_In_First_Out 후입선출)

// [1][2][3][4] << 밀어넣음
// ex) 되돌리기 (Ctrl + Z)


template<typename T, typename Container = vector<T>>
class Stack
{
public:
	void push(const T& value)
	{
		_container.push_back(value);
	}

	void pop()
	{
		_container.pop_back();
	}

	T& top()
	{
		return _container.back();
	}

	bool empty() { return _container.empty(); }
	int size() { return _container.size(); }

private:
	Container _container;
};

int main()
{
	// 스택 생성 
	Stack<int> s;

	// 데이터 밀어넣기
	s.push(1);
	s.push(2);
	s.push(3);

	while (s.empty() == false)	// 스택이 비었나?
	{
		// 최상위 원소가 무엇?
		int data = s.top();

		// 최상위 원소를 삭제
		s.pop();

		cout << data << endl; 
	}

	// 사이즈는?
	int size = s.size();
}