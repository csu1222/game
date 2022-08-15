#include <iostream>
using namespace std;

// 오늘의 주제 : 템플릿 기초

/*
	템플릿 : 함수나 클래스를 찍어내는 틀
	1) 함수 템플릿
	2) 클래스 템플릿  # 
	
	클래스 템플릿은 함수 템플릿과 비슷하게 template<typename T> 로 타입 네임을 이름 짓고 
	타입부분에 템플릿을 써주면 됩니다. 사용할때 함수때와 조금 다른것이 
	함수는 사용할때 인자가 바로 들어가서 명시적으로 타입 지정을 안해줘도 됐지만
	클래스 생성의 경우 인자를 넣어주는 생성자가 아닌 그냥 생성하는 타입의 경우 명시적으로 
	어떤 타입의 클래스로 만들것인지 알려줘야 겠습니다.

	그런데 무조건 typename을 붙여야 하는 것은 아니다?
	typename만 가변적으로 사용되는것이 아니라 민의 RandomBox클래스의 경우 배열의 크기도 가변적으로 가능해집니다.
	templae< > 안에 들어가는건 [골라줘야 하는 목록]이라고 볼 수 있음 
	template<typename T, int SIZE>  이렇게 타입네임과 SIZE라는 int값을 템플릿으로 만들어서 
	배열의 값 타입, 배열의 크기를 각각 T, SIZE로 해주었습니다. 
	이 클래스 템플릿은 각각
	RandomBox<int, 10> rbx1;  이런 식으로 타입과 배열의 크기를 골라줘야합니다.

	그러면 클래스 템플릿으로 만든 클래스와 어떤 인자를 받는 함수와의 차이점은
	함수를 여러 인자로 여러번 호출하는것은 하나의 함수주소로 인자만 바뀌면서 실행되는것이고
	클래스 템플릿으로 템플릿을 바꿔가면서 호출하는것은 각 한번 호출때마다 새로운 클래스를 생성하는 것입니다.

	함수 템플릿 에서도 배운 템플릿 특수화를 클래스 템플릿에도 사용 할 수 있습니다. 
	템플릿 특수화를 다시 설명하자면, 어떤 특정 타입으로 템플릿을 사용하면 예외적인 형태의 클래스를 만들어 주는 것입니다.
	템플릿 특수화 문법은 함수 템플릿때와 비슷하기도 하고 조금 다르기도 합니다.
	template<int SIZE>		 함수 템플릿때 처럼 typename을 비워줍니다. int SIZE는 특수화랑 별개로 그냥 씁니다.
	
	class RandomBox<double, SIZE>
	{	
	구현부
	}
	함수 템플릿 특수화와 다르게 클래스 이름에 어떤 타입이 오면 특수화가 될것인지 시그니쳐를 적어줘야합니다.
*/

template<typename T, int SIZE>
class RandomBox
{
public:
	T GetRandomData()
	{
		int idx = rand() % SIZE;
		return _data[idx];
	}

public:
	T _data[SIZE];
};

// 템플릿 특수화
template< int SIZE>
class RandomBox<double, SIZE>
{
public:
	double GetRandomData()
	{
		cout << "randomdox double" << endl;
		int idx = rand() % SIZE;
		return _data[idx];
	}

public:
	double _data[SIZE];
};

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	RandomBox<int, 10> rb1;

	for (int i = 0; i < 10; i++)
	{
		rb1._data[i] = i;
	}
	int value1 = rb1.GetRandomData();
	cout << value1 << endl;

	RandomBox<double, 20> rb2;

	for (int i = 0; i < 20; i++)
	{
		rb2._data[i] = i + 0.5;
	}
	double value2 = rb2.GetRandomData();
	cout << value2 << endl;


	return 0;
}