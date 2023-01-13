#include <iostream>
#include <vector>
using namespace std;


template<typename T>
class Vector
{
public:
	Vector()
	{

	}
	~Vector()
	{
		if (_data)
			delete[] _data;
	}

	void push_back(const T& value)
	{
		// 현재 사이즈와 여유공간이 같은지 체크 
		if (_size == _capacity)
		{
			// 증설 작업 
			int NewCapatity = static_cast<int>(_capacity * 1.5f);
			if (NewCapatity == _capacity)	// 처음에 사이즈가 작을때 
				NewCapatity++;

			reserve(NewCapatity);
		}

		// 데이터 저장
		_data[_size] = value;

		// 데이터 개수 증가 
		_size++;
	}

	void reserve(int capacity)
	{
		if (_capacity >= capacity)
			return;

		_capacity = capacity;

		// 새로 할당
		T* NewData = new T[_capacity];

		// 데이터 복사
		for (int i = 0; i < _size; i++)
			NewData[i] = _data[i];

		// 기존 데이터 삭제
		if (_data)
			delete[] _data;

		// 새 동적 배열을 교체
		_data = NewData;
	}

	T& operator[](const int pos) { return _data[pos]; }

	int size() { return _size; }

	int capacity() { return _capacity; }

	void clear()
	{
		if (_data)
		{
			delete[] _data;
			_data = new T[_capacity];
		}

		_size = 0;
	}

private:
	T*		_data = nullptr;
	int		_size = 0;
	int		_capacity = 0;
};

int main()
{
	vector<int> V;

	V.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		V.push_back(i);

		cout << V[i] << " " << V.size() << " " << V.capacity() << endl;

	}

	V.clear();
	cout << V.size() << " " << V.capacity() << endl;
}