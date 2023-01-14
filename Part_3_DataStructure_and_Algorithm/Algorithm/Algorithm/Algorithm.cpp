#include <iostream>
#include <vector>
#include <list>
using namespace std;

template<typename T>
class Node
{
public:
	Node() : _prev(nullptr), _next(nullptr), _data(T())
	{

	}

	Node(const T value) : _prev(nullptr), _next(nullptr), _data(value)
	{

	}

public:
	Node*	_prev;
	Node*	_next;
	T		_data;
};

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

	// it++
	Iterator operator++(int)
	{
		Iterator temp = *this;
		_node = _node->_next;

		return temp;
	}
	// ++it
	Iterator& operator++()
	{
		_node = _node->_next;

		return *this;
	}
	// it--
	Iterator operator--(int)
	{
		Iterator temp = *this;
		_node = _node->_prev;

		return temp;
	}
	// --it
	Iterator& operator--()
	{
		_node = _node->_prev;

		return *this;
	}

	// *it
	T& operator*()
	{
		return _node->_data;
	}

	// == 
	bool operator==(const Iterator& other)
	{
		return _node == other._node;
	}
	
	// !=
	bool operator!=(const Iterator& other)
	{
		return _node != other._node;
	}

public:
	Node<T>* _node;
};

template<typename T>
class List
{
public:
	List() : _size(0)
	{	
		// [head] <-> ... <-> [tail]
		_head = new Node<T>();
		_tail = new Node<T>();

		_head->_next = _tail;
		_tail->_prev = _head;
	}

	~List()
	{
		while (_size > 0)
			pop_back();

		delete _head;
		delete _tail;
	}

	void push_back(const T& value)
	{
		AddNode(_tail, value);
	}

	void pop_back()
	{
		RemoveNode(_tail->_prev);
	}

	int size() { return _size; }

private:

	// [head] <-> [1] <-> [prevNode] <-> [before] <-> [tail]
	// [head] <-> [1] <-> [prevNode] <-> [NewNode] <-> [before] <-> [tail]
	Node<T>* AddNode(Node<T>* before, const T& value)
	{
		Node<T>* newNode = new Node<T>(value);
		Node<T>* prevNode = before->_prev;

		prevNode->_next = newNode;
		newNode->_prev = prevNode;

		newNode->_next = before;
		before->_prev = newNode;

		_size++;

		return newNode;
	}

	// [head] <-> [prevNode] <-> [node] <-> [nextNode] <-> [tail]
	// [head] <-> [prevNode] <-> [nextNode] <-> [tail]
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

public:
	using iterator = Iterator<T>;

	iterator begin() { return _head->_next; }
	iterator end() { return _tail; }

	// insert : 'it' 앞에 추가 
	iterator insert(iterator it, const T& value)
	{
		Node<T>* node = AddNode(it._node, value);
		return iterator(node);
	}

	iterator erase(iterator it)
	{
		Node<T>* node = RemoveNode(it._node);
		return iterator(node);
	}

private:
	Node<T>*	_head;
	Node<T>*	_tail;
	int			_size;
};

int main()
{
	List<int> li;

	List<int>::iterator eraseIt;
	for (int i = 0; i < 10; i++)
	{
		if (i == 5)	// insert 는 이터레이터를 주면 그 앞에 데이터를 넣습니다. 
		{
			eraseIt = li.insert(li.end(), i);
		}
		else
		{
			li.push_back(i);
		}
		// list 에는 특이하게 push_front 도 있습니다. 

	}
	li.pop_back();

	li.erase(eraseIt);

	for (List<int>::iterator it = li.begin(); it != li.end(); it++)
	{
		cout << (*it) << endl;
	}
	// vector 
	// - push_back	O(1)
	// - push_front O(n)
}