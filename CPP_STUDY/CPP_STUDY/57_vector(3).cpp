#include <iostream>
using namespace std;
#include <vector>

// 오늘의 주제 : vector - 3

/*
vector를 마무리 할겸 vector를 구현해보는 연습
*/

template<typename T>
class Iterator
{
public:
	Iterator() : _ptr(nullptr)
	{

	}

	Iterator(T* ptr) : _ptr(ptr)
	{

	}

	Iterator& operator++ () // 전위형
	{
		_ptr++;
		return *this;
	}

	Iterator operator++(int) // 후위형
	{
		Iterator temp = *this;
		_ptr++;
		return temp;
	}

	Iterator& operator-- () // 전위형
	{
		_ptr--;
		return *this;
	}

	Iterator operator--(int) // 후위형
	{
		Iterator temp = *this;
		_ptr--;
		return temp;
	}

	Iterator operator+ (const int count)
	{
		Iterator temp = *this;
		temp._ptr += count;
		return temp;
	}

	Iterator operator- (const int count)
	{
		Iterator temp = *this;
		temp._ptr -= count;
		return temp;
	}

	bool operator== (const Iterator& right)
	{
		return _ptr == right._ptr;
	}

	bool operator!= (const Iterator& right)
	{
		return  _ptr != right._ptr;  // 또는 !(*this == right) 로 위에 구현한 == 연산자를 재사용합니다.
	}

	T& operator* ()
	{
		return *_ptr;
	}

public:
	T* _ptr;
};

template<typename T>
class Vector
{
public:
	Vector() : _data(nullptr), _size(0), _capacity(0)
	{

	}
	~Vector()
	{
		if (_data)
			delete[] _data;
	}

	void push_back(const T& val)
	{
		if (_size == _capacity)	// 둘이 같다라는 것은 꽉찼다는 의미
		{
			// 증설작업
			int newCapacity = static_cast<int>(_capacity * 1.5);
			if (newCapacity == _capacity)
				newCapacity++;

			reserve(newCapacity);
		}
		// 데이터 저장
		_data[_size] = val;
		// 데이터 개수 증가
		_size++;

	}

	void reserve(int capacity)
	{
		_capacity = capacity;

		T* newData = new T[_capacity];

		// 데이터 복사작업
		if (_data)
		{
			for (int i = 0; i < _size; ++i)
				newData[i] = _data[i];
		}
		// 기존에 있던 데이터를 날린다
		if (_data)
			delete[] _data;

		_data = newData;
	}

	void clear() { _size = 0; }

	T& operator[] (const int pos)
	{
		return _data[pos];
	}

	int size() { return _size; }
	int capacity() { return _capacity; }

public:
	typedef Iterator<T> iterator;

	iterator begin() { return iterator(&_data[0]); }
	iterator end() { return  &(_data[_size]) /*begin() + _size*/; }

public:
	T* _data;
	int _size;
	int _capacity;
};

int main()
{

	// 구현 목표 : 밑의 기능들을 구현해보기
	Vector<int> v;
	
	v.reserve(100);

	for (int i = 0; i < 100; ++i)
	{
		v.push_back(i);
		cout << v.size() << " " << v.capacity() << endl;
	}

	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << endl;
	}

	cout << "----------------" << endl;

	for (Vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		cout << (*it) << endl;
	}

	v.clear();


	return 0;
}