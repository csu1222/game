#include <iostream>
using namespace std;
#include <vector>

// ������ ���� : vector - ���� �ܰ�

/*
	�̹� �ܿ��� STL�̰� �� �� ù��° vector �ð��Դϴ�.
	�켱 STL �� �����̳�?
	STL(Standard Template Library)
	���α׷����� �� �ʿ��� �ڷᱸ��/�˰�������� 
	���ø����� �����ϴ� ���̺귯��

	�����̳�(Container) : �����͸� �����ϴ� ��ü (�ڷᱸ�� Data Structure)

	vector (���� �迭)
	- vector�� ���� ���� (size/capacity)
	- �߰� ����/����
	- ó��/�� ����/����
	- ���� ����

	vector���� ���� �迭�� ���α׷��� ����� �ٸ��̸����ζ� �ϳ��� ������ �ֽ��ϴ�. 
	���� �迭�� �� ����� �ϴ��� �迭�� ������ ���� �˾ƺ��ڽ��ϴ�.

	�迭
	- �迭�� ó�� ���鶧 ��� ũ�� ��ŭ�� �迭�� ��������� ���ϰ������� �� �迭�� ũ�⸦ ���̰ų� ���ϼ� �����ϴ�.

	vector�� �⺻ ������� ���� �˾ƺ��ڽ��ϴ�. 
	�켱 #include <vector> �� ���ݴϴ�.
	vector�� ���ø����� ��������־ 
	vector<int> v;		��� ����� int�� ���� vector v�� ��������ϴ�. 
	������ std::vector<int> v; 
	��� ������ ������ �츮�� ���� using namespace std; �� ����ϰ� �־� �����Ǿ����ϴ�.

	���ʹ� �ű��ϰԵ� ũ�Ⱑ �þ�ٰ� �پ����ٰ� �ϴ� Ư¡�� �ֽ��ϴ�.
	������ ���ĵΰ� ������ ����غ��Խ��ϴ�. ���� ���� ��ü v�� .�� ���� ����Ҽ� �ִ� ��ɵ��� ���� �����µ�
	���� push_back�� �������� ������ push ���ִ� ����Դϴ�. 
	1,2,3,4,5�� push_back���ְ� 
	����� �غ��߰ڽ��ϴ�. �����迭�� �迭ó�� [] �� �����ڸ� �����մϴ�. �׸��� �� �迭�� ũ�⸦ ������ �ִ�
	v.size(); ���̿��� for���� ���� �Ϲ� �迭�� �Ȱ��� �ϳ��� ����� �ټ� �־����ϴ�.

	## �ſ� �߿��� ����
	- � ������ �ηȱ淡 �迭�� '������'���� ����� ���ΰ�?
	vector�� ������ �ΰ��� ������ �ִµ�
	1) (�������� �ΰ�) �޸𸮸� �Ҵ��Ѵ� 
	2) ������ ���� �� á����, �޸𸮸� �����Ѵ�.

	Q1) �������� ��ŭ�� �����ұ�?
	Q2) ������ ��ŭ �ؾ� �ұ�?
	Q3) ������ �����͸� ��� ó���ұ�?

	Q1, Q2�� ���� ���� �Ҽ� ������ �����ϴ�. int�� vector���� �ƹ� ���� 1000�� �־����ϴ�. 
	���� ���������� size, capacity�� ����߽��ϴ�. �� �ݺ����� size�� 1�� �ö����� capacity�� 1�� �ö���� �ʰ�
	1 2 3 4 6 9 13 19 28 42 
	�̷��� �ö󰡰� �־����ϴ�. ��Ģ�� ���� �������� ���� ���� ���� capacity�� 1.5�迡 �ٻ��ϰ� �ø��� �ֽ��ϴ�.
	(������ �󸶳� �Ǵ����� �����Ϸ����� �ٸ��ٰ� �մϴ�.)
	[size : ���� ����ϰ� �ִ� �������� ����]
	[capatity : �������� ������ �������� ����]

	�׷��ٸ� �� 1.5�辿 ������ �ΰ� ������ �ұ��? �׳� size�� ���� 1���ø��� �ȵɱ��?
	�� ������ Q3) �� ������ �ֽ��ϴ�.
	Q3) ������ �����͸� ��� ó���ұ�? 
	- �����ϰ� �����ϸ� �迭�� �����ϸ鼭 malloc���� �ɷ� �޸𸮸� Ȯ���ؼ� ������ �����Ϳ� �̾ ó���ϸ� ���� ������?
	-- �迭�� Ư¡�� �޸𸮻����� ���ӵǾ� �ϴµ� �迭�ڿ� � �����Ͱ� �������� �𸣴� �޸𸮸� Ȯ���Ҽ� �����ϴ�.
	- �׷��� �޸𸮸� �����Ҷ� �����Ҹ�ŭ�� ������ �ִ� �޸𸮸� ã�� �� �迭�� ����� ������ �����͸� �����ؿͼ� ��
	�迭�� �������°� �Դϴ�.
	�� ������ �����͸� �����ؿ��� ������ �����Ҷ����� �̷����� �ϴϱ� �������� �ΰ� ������ �Ǵ� ���Դϴ�.

	�׻� capacity�� �����ǰ� �ؾ� �ϴ� ���� �ƴϰ� ���� �츮�� ���� �������� ������ ���� �˰� �ִٸ� �̸� �������� ������
	������ �ֽ��ϴ�.
	.reserve(������); ���� �̸� ������ ������ �ֽ��ϴ�.
	reserve�� capacity�� �̸� �����س��°Ŷ�� size(���� ����ϰ� �ִ� �������� ����)�� �̸� �����ϴ� ����� �ֽ��ϴ�.
	resize(); ���״�� ����� �ٽ� ���ϴ� ����Դϴ�.
	resize(1000); ���� size�� �ø� ������ size�� capacity�� ����غ��� �Ѵ� 1000���� ���ɴϴ�. 

	�׷��� resize�� push_back���� ���빰�� �߰��ϸ� �̸� ��Ƴ��� �޸𸮿� �����?
	�ƴմϴ�. resize�� �κ��� �̹� �����Ͱ� �ڸ���� �ְ� �׵ڿ� �������� ���빰�� ���ϴ�. 
	resize�� Ȯ���� �κп� ���ϴ� ���빰�� �������� v[i] �̷��� �迭���� �� �ε����� ���빰�� �ֵ��� �ϸ�ǰڽ��ϴ�.

	�� vector(���� �迭)�� ����ϴٺ��� ���� ����ϴ� ������ �� size�� �پ�鶧�� ���� ���Դϴ�. 
	capacity�� �׷��� ���� size�� ���� �پ���� �ʽ��ϴ�. 
	vector�� ���빰�� �� ���ִ� ����� clear�� ����� size�� capacity�� ����غ��� size�� 0, capacity�� �״���ΰ� 
	�˼� �ֽ��ϴ�.

	�Ҽ��� ������ capacity�� ����ϰ� ���ֹ����� �ʹ�! ���
	vector<int>() �� �ӽ� ���͸� ����ϴ�.
	vector<int>().swap(v); �ӽú��Ϳ� �츮�� ����ϴ� ���͸� �����ϸ� �츮�� ����ϴ� ���ʹ� ��ݸ��� �ƹ������͵� ����
	���Ͱ� �Ǵ°��̰� capacity�� �þ �ִ� ���ʹ� �ӽú��Ͱ� ���������� �� �ڵ� ������ ������ ���� �����Ǿ �޸� ������ �˴ϴ�. 

	�̿ܿ��� ���� ��ɵ����ֽ��ϴ�.
	v.front(); : �Ǿտ� �ִ� ���� ����
	v.back();  : �ǵ��� ���� ����
	v.pop_back : push_back�� �ݴ�� �� �������� ���� �̾Ƴ���

	���͸� ���鶧 �ٷ� resize�� ���� ���� �ֽ��ϴ�.
	vector<int> v(1000);   : ���͸� ����� ���ÿ� resize(1000)�� ���ذͰ� �����մϴ�.
	vector<int> v(1000, 0) : �ι�°���ڷ� �������� �迭�� �ʱⰪ���� �������� �� �ֽ��ϴ�. (���빰�� ���� 0���� �ʱ�ȭ)
	
	vector<int> v2 = v;    : ���θ���� ���Ϳ� �̹� �ִ� ������ ���µ��� �����մϴ�.
*/



int main()
{
	// �迭
	//const int MAX_SIZE = 10;
	//int arr[MAX_SIZE] = {};

	//for (int i = 0; i < MAX_SIZE; i++)
	//	arr[i] = i;

	//for (int i = 0; i < MAX_SIZE; i++)
	//	cout << arr[i] << endl;


	// vector

	vector<int> v;
	//v.resize(1000);
	v.reserve(1000);
	for (int i = 0; i < 1000; i++)
	{
		//v[i] = 100;
		 v.push_back(100);
		cout << v.size() << " " << v.capacity() << endl;
	}
	// size(���� ��� ������ ����)
	// 1 2 3 4 5 6 ...
	// capacity(�������� ������ �뷮 ����)
	// 1 2 3 4 6 9 13 19 28 42 ...

	v.clear();
	vector<int>().swap(v);
	cout << v.size() << " " << v.capacity() << endl;


	return 0;
}