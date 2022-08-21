#include <iostream>
using namespace std;
#include <vector>
#include <list>

// 오늘의 주제 : list - 2 - 구현

// 우선 리스트의 구성요소인 Node의 클래스를 만들었습니다.
template<typename T>
class Node
{
public:
	Node() : _next(nullptr), _prev(nullptr), _data(T())
	{

	}

	Node(const T& value) : _next(nullptr), _prev(nullptr), _data(value)
	{

	}

public:
	Node*	_next;
	Node*	_prev;
	T		_data;
};


// iterator
template<typename T>
class Iterator
{
public:
	Iterator() : _node(nullptr)
	{

	}

	Iterator(Node<T>* node) : _node(node)
	{

	}

	//++it
	Iterator& operator++ ()
	{
		_node = _node->_next;

		return *this;
	}

	//it++
	Iterator operator++ (int)
	{
		Iterator<T> temp = *this;
		_node = _node->_next;
		return temp;
	}

	//--it
	Iterator& operator-- ()
	{
		_node = _node->_prev;

		return *this;
	}

	//it--
	Iterator operator-- (int)
	{
		Iterator<T> temp = *this;
		_node = _node->_prev;
		return temp;
	}

	T& operator* ()
	{
		return _node->_data;
	}

	bool operator== (const Iterator& right)
	{
		return _node == right._node;
	}

	bool operator!= (const Iterator& right)
	{
		return _node != right._node;
	}

public:
	Node<T>* _node;
};

//		<-> [ header] <->
// [1] <-> [2] <-> [3] <-> [4] <-> [ header ] <->
template<typename T>
class List
{
public:
	List() : _size(0)
	{
		// _header는 아무 값이 없어도 존재하는 더미 노드입니다. end()의 반환 값이 이 _header입니다.
		_header = new Node<T>();
		_header->_next = _header;
		_header->_prev = _header;
		// 이부분은 처음생성한 list의 헤더는 혼자 있으므로 next, prev 전부 자신을 가리키고 있습니다.
	}

	~List()
	{
		while (_size > 0)
			pop_back();

		delete _header;
	}

	void push_back(const T& value)
	{
		AddNode(_header, value);
	}

	// [1] <-> [2] <-> [3] <-> [4] <-> [ header ] <->
	// [1] <-> [2] <-> [3] <-> [ header ] <->
	void pop_back()
	{
		RemoveNode(_header->_prev);
	}

	//		<-> [ header] <->
	// [1] <-> [2] <-> [node] <-> [before] <-> [4] <-> [ header ] <->
	Node<T>* AddNode(Node<T>* before, const T& value)
	{
		Node<T>* node = new Node<T>(value);

		Node<T>* prevNode = before->_prev;
		prevNode->_next = node;
		node->_prev = prevNode;

		node->_next = before;
		before->_prev = node;

		_size++;

		return node;
	}

	// [1] <-> [2] <-> [node] <-> [4] <-> [ header ] <->
	// [1] <-> [2] <-> [3] <-> [ header ] <->
	Node<T>* RemoveNode(Node<T>* node)
	{
		Node<T>* prevNode = node->_prev;
		Node<T>* nextNode = node->_next;

		prevNode->_next = nextNode;
		nextNode->_prev = prevNode;

		delete node;

		_size--;

		return nextNode;
	}

	int size() { return _size; }
	
	typedef Iterator<T> iterator;

	iterator begin()
	{
		return iterator(_header->_next);
	}

	iterator end()
	{
		return iterator(_header);
	}

	iterator insert(iterator it, T& value)
	{
		Node<T>* node = AddNode(it._node, value);
		return iterator(node);
	}

	iterator erase(iterator it)
	{
		Node<T>* node = RemoveNode(it._node);
		return iterator(node);
	}

public:
	Node<T>* _header;
	int _size;

};

int main()
{

	List<int> li;

	List<int>::iterator eraseIt;


	for (int i = 0; i < 10; i++)
	{
		if (i == 5)
		{
			eraseIt = li.insert(li.end(), i);
		}
		else
		{
			li.push_back(i);
		}
	}

	li.pop_back();

	li.erase(eraseIt);

	for (List<int>::iterator it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	return 0;
}