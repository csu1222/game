#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
// ������ ���� : deque

/*
	������ �����̳� (Sequence Container)
	�����Ͱ� ������� �����Ǵ� ������ �����̳�
	vector, list, deque

	vector : �����迭
	list : ���� ���� ����Ʈ
	
	deque : double-ended queue ��ũ
		�ϴ� ����ó�� �迭�� ���·� �����͸� �����ϴٰ� ������ �Ǵٸ� �޸𸮿� �ٽ� �迭�� ���� �̾� �����մϴ�.
		�ٸ� �迭�� ���鶧 ����ó�� ���� ������ ��� �����ذ��°��� �ƴ϶� �׳� ���� ������ �����մϴ�. �� �迭�� 
		����Ʈó�� �����ͷ� ����ŵ�ϴ�.

		push_back�� push_front�� ���캸�� 
		push_back���� ó�� ���� �迭�� ������ �Ǹ� ���� �迭�� �ΰ� �ٸ� �޸𸮿� ���� �迭�� ����� �̾����� ���� �����մϴ�.
		
		push_front�� �տ� ���� �־��ָ� ���� �迭�� �������� �̾����� �迭�� ����� �ű⿡ push_front�� ���� ����, �ε�����0����
		�����մϴ�.

	- deque ���ۿ���
	- �߰� ����/���� (BAD / BAD)
	- ó��/�� ����/���� (GOOD / GOOD)
	- ���� ����  (GOOD)


	�������� : deque�� [ ] �����ڷ� ���� ������ �����߽��ϴ�. vectoró�� ������ ���ӵ� �޸� ���� �ƴѵ� ��� �������� 
		�˾ƺ���, deque�� f12�� ���� �ڵ忡 �����ؼ� operator[] �� ã�ҽ��ϴ�. [] �����ڴ� 
		return *(_Unchecked_begin() + static_cast<difference_type>(����));
		�� ������ �Ǿ��־����ϴ�. ���� _Unchecked_begin()�� ���ͷ����Ͱ� �� ���ͷ����Ϳ� ���ڸ�ŭ +�������ؼ� �װ��� *(������)
		�� ��ȯ�ϴ� �� �����ϴ�.
		�׷��� deque�� ���ͷ����͸� �ٽ� �˾ƺ��߰ڽ��ϴ�.

		deque�� ���ͷ����� �ڵ忡�� operator* �� �����θ� ����
		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % _Block_size;
		return _Mycont->_Map[_Block][_Off];

		�������� ������ �迭���� _Block���� �θ��� ��ϸ��� ��ȣ�� �ְ� , ����� ũ�⸦ _Block_size��� �ϴ°� �����ϴ�.
		_Myoff�� ���ϰ��� �ϴ� ���� ���° ��ġ�� �ִ��� �̰�, _Off ������ _Myoff�� ��ϳ��� �ε����� ���� �ؼ�  
		_Getblock���� ���° ������� ��� 
		_Mycont->_Map[_Block][_Off]; �� ���� �� �𸣰����� _Map�̶�� ������� ���° ��Ͽ� ��� �ε����� ���� ��ȯ�ϴ�
		������ �����ε� �����ϴ�.

		�̷� ������� ���� ������ ������ deque�� �� ����� �޸𸮰� ������� �ʴٰ� Ȯ�� �Ҽ� �־�� ������ ���������ϴ�. 
		�׷� ������ �߰� ���� ������ ����� ���Դϴ�. 

		�׸��� deque�� double_ended queue, �յڷ� ���� ������ ������ queue��� �ǹ��ε� 
		�� queue�� ������ ���� �� ���Դϴ�.
	 
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