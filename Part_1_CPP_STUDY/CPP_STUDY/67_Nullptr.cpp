#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

// ������ ���� : nullptr

/*
* ��� : ���ݱ��� ����ߴ��� ó�� nullptr�� �������
* 0, NULL �� nullptr�� ������ :
	- 0, NULL�� ������ 0�� �ּҰ� 0�� ȥ���Ǿ� ���ɼ� �ֽ��ϴ�. 
	- nullptr�� ��ü�� ��ü�̰� �� ��ü�� �� �����͵�� �񱳸� �ϰ� �Ǹ� �� �����ͷ� ĳ������ �Ǹ鼭
	��ȯ ���� 0�� ��ȯ�մϴ�. �׷��� �Ǽ� ���� 0���� ȥ������ �ʰ� ���ø� Ÿ���� �����ͷν� ����մϴ�.
	
	nullptr�� ��� c++���� ��Ÿ���� �������� ���ݲ� 
	�ڿ������� ����ؿԽ��ϴ�. 

	�׷��� nullptr�������� ��� �ּҰ� ���ٴ°� ǥ���������?
	�ٷ� 0�� NULL�� �̿��Ͽ����ϴ�.

	�����Ϳ� ������ 0�� �ָ� �ʹ� �������� ������ �߰����� �ֱ� ������ NULL�ε� ǥ���� �Ѱ��Դϴ�.
	NULL�� 
	#define NULL 0; 
	�̶�� �׳� 0�� ���� �ǹ����� ������ �����Ͷ�°� �� �˾ƺ��Բ� �մϴ�.

	nullptr�� �ʿ伺
	1) ������
	0�� NULL�� �������� �����ϱ� nullptr�̶����� �ʿ����� ���Դϴ�.
	�Լ� �����ε��Ҷ� ������ ����ϴ�.
	�Ű������� int�� �޴� �Լ��� void*�� �޴� �Լ��� �����ε�������
	���ڷ� 0�� NULL�� �ְ� �����ϸ� �Ѵ� �Ű������� intŸ���� �Լ��� ����˴ϴ�.
	NULL�� �����Ͷ�°� ǥ���ϱ� ���� define�̾����� ���� ���� �׳� 0���� int�� ����� �Ǵ°��Դϴ�.

	nullptr�� ����ϸ� ���������� ������ ���� �Լ��� ����˴ϴ�. 
	��, ������ ������ ���� �ǹ̵� �ְڽ��ϴ�.

	2) ������
	�ڵ忡�� auto Ÿ������ ������ ��������� �� ������ Ÿ���� nullptr�� ���ϰ� �Ǹ� �ٷ� �� ������ 
	������Ÿ�� ������°��� �˼� �ֽ��ϴ�.


	nullptr�� ���� ������� �ִ� �͵�� �ٸ��� ���� nullptr�� ����ϸ� �� �����Դϴ�.

	�׷��� nullptr�� ��� �����Ǿ� �ֱ淡 ������ �ִ� �����ϱ��
	���ø��� �̿��ؼ� ������ �����ε��� ���ָ� 
	��ȯŸ���� �������� �ʾƵ� �˴ϴ�.
	
	operator T* () const{return 0;} 
	���� T��� ���ø����� � Ÿ���� �͵� �Ǵϱ� � Ÿ���� �����Ϳ� �� nullptr�̶�� ��ü�� ���ϸ� nullptr��
	���ϴ� �������� Ÿ������ ĳ������ �ǰ� 0�� ��ȯ���༭ ��� 0�� ���ϰ� �˴ϴ�.
	const�� ������ �� ������ �����ε��� �����ΰ� �ٲ��� �������̶�� �� ���մϴ�.

	operator T C::* () const {return 0;}
	�� T�� C��� ���ø��� �ְ� ��ȯ Ÿ���� T�� � ��ȯŸ���̵� C��� � Ŭ���������� ::��� �Լ��� *�����Ϳ� 
	nullptr�� ���ϸ� nullptr�� �� ��� �Լ� �������� Ÿ������ ĳ������ �Ǹ鼭 0�� ��ȯ, �� ��� �Լ������Ϳ� 
	0�� ���ϰ� �˴ϴ�.	
	���°� �ſ� Ư�������� ���÷δ� ��� �Լ� �����Ͱ� �ֽ��ϴ�.

	�׸��� 
	private: �� �ܺο��� �����������ϰ� 
	void operator& () const;
	�� �����ξ��� ������ �ع����� nullptr�� �ּҿ� ������ ���ϰ� ������ �ֽ��ϴ�.
	��� private�� ������ ���°� ���� ��Ÿ���̰� C++ 11 �������ʹ�
	void operator& () const = delete;
	�̷��� ó���Ѵٰ� �մϴ�. 


*/
// nullptr �����ϱ�
const
class NullPtr
{
public:
	// �� � Ÿ���� �����Ͷ� ġȯ ����
	template<typename T>
	operator T* () const
	{
		return 0;
	}

	template<typename C, typename T>
	operator T C::* () const
	{
		return 0;
	}

private:
	void operator& () const;	// �ּҰ� %�� ���´� ���� private�� ���Ƽ� ������ ���°�
} _NullPtr;	// Ŭ������ ���� ���ڸ��� ��ü�� ������ִ� ����


// 0�� NULL�� ������  (�Լ� �����ε�)
void test(int a)
{
	cout << "test (int)" << endl;
}

void  test(void* ptr)
{
	cout << "test(*)" << endl;
}

class Knight
{
public:
	int nullptr_test(int)
	{
		cout << "member function pointer" << endl;
		return 1;
	}
public:
	int _hp;
	int* _hpPtr;
};

Knight* FindKnight()
{
	// TODO

	return _NullPtr;
}


int main()
{
	// 0�� NULL�� ������  

	// 1) ������
	test(0);
	test(NULL);
	// �Ѵ� �Ű������� int�� �Լ��� ����Ǿ����ϴ�.
	test(_NullPtr);
	// ���������� ������ ���� �Լ��� ����˴ϴ�.

	// 2) ������
	// autoŸ������ ó���� knight������ ���� Ÿ������ nullptr�� ���� �ٷ� ������ Ÿ���ΰ� �˼� �ֽ��ϴ�.
	auto knight = FindKnight();
	if (knight == _NullPtr)
	{

	}
	Knight k1;


	// ��� �Լ� ������ 
	int (Knight:: * memFUNC)(int);
	memFUNC = &Knight::nullptr_test;

	if (memFUNC == _NullPtr)
	{

	}



	return 0;
}