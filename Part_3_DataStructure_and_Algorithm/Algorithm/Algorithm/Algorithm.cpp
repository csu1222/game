#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

template<typename T, typename Container = vector<T>, typename Predicate = less<T>>
class Priority_queue
{
public:
	void push(const T& value)
	{
		// 우선 힙 구조 부터 맞춰준다.	
		_heap.push_back(value);

		// 도장깨기 시작
		int now = static_cast<int>(_heap.size()) - 1;

		// now > 0 이란것은 루트노드까지 도달하면 종료
		while (now > 0)
		{
			// 부모 노드와 비교해서 더 작으면 패배
			int next = (now - 1) / 2;

			/*if (_heap[now] < _heap[next])
				break;*/
			if (_predicate(_heap[now], _heap[next]))
				break;

			// 데이터 교체 
			::swap(_heap[now], _heap[next]);
			now = next;
			_size++; 
		}
	}

	T& top()	{ return _heap[0];	}

	void pop()
	{
		_heap[0] = _heap.back();
		_heap.pop_back();

		int now = 0;

		while (true)
		{	
			int left = now * 2 + 1;
			int right = now * 2 + 2;

			// 리프에 도달한 경우 
			// 일단 왼쪽과 비교 
			if (left >= _heap.size())
				break;

			int next = now;

			if (_predicate(_heap[next], _heap[left]))
				next = left;

			// now 와 left 중 승자가 이번엔 오른쪽과 비교 

			// 그 전에 right 가 유효한지 체크 
			if (right < _heap.size() && _predicate(_heap[next], _heap[right]))
				next = right;

			// 왼쪽/오른쪽 두 값모두 현재값 보다 작으면 종료
			if (next == now)
				break;
			
			// 데이터 스왑
			::swap(_heap[now], _heap[next]);
			now = next;
		}
	}

	bool empty() { return _heap.empty(); }

	int size() { return _size; }
private:
	Container	_heap = {};
	Predicate	_predicate = {};
	int			_size = 0;
};

int main()
{
	Priority_queue<int, vector<int>, greater<int>> pq;

	pq.push(100);
	pq.push(300);
	pq.push(200);
	pq.push(500);
	pq.push(400);

	while (pq.empty() == false)
	{
		int value = pq.top();
		pq.pop();

		cout << value << endl;
	}
}