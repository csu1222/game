#include <iostream>
using namespace std;

// ������ ���� : ���� 

#pragma region ������ ���� ������ �ʿ伺
// ������ �����Ϳ� ������ ������ �ֽ��ϴ�.
// ���� �ǽ��� �����ϸ鼭 ������ �ʿ伺�� �˾ƺ��ڽ��ϴ�.

struct StatInfo
{
	int hp;			// +0
	int attack;		// +4
	int defence;	// +8
};

// [�Ű�����][RET][��������(info)] [�Ű�����(&info)][RET][��������]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 15;
	info->defence = 5;
}

// ���� �������� �ʴ´ٸ� �Ű������� �����͸� �ִ°� �ƴ� ���� �������ִ°͵� ������ �����ϴ�.
// ����� ������ ���� �������� ���̰� �ֽ��ϴ�. 
// �� ���� ����� ��� �Ű������� ���� ���� ������ ���������� �����Ͽ� ����� ��
// �ּ� ���� ����� ��� �Ű������� ���� �����Ͱ� ����Ű�� �ּҸ� ���� ����� ��
// 
// StatInfo ����ü�� 1000����Ʈ¥�� ���� ����ü���?
// - (�� ����) StatInfo�� �ѱ�� 1000����Ʈ�� ����Ǵ�
// - (�ּ� ����) StatInfo*�� �ѱ�� 8����Ʈ(�ּ��� ����)
// - (���� ����) ��� 3��° �ɼ����� �̹� ������ ���������� �ִٰ� �մϴ�.
// -  �� StatInfo&�� �ѱ�� 8����Ʈ(�ּ��� ����)
// 
// 1) �� ���� ���
void PrintInfoByCopy(StatInfo info)
{
	cout << "-------�� ���� ��� -------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.attack << endl;
	cout << "DEF : " << info.defence << endl;
	cout << "---------------------------" << endl;
}

// 2) �ּ� ���� ���	
void PrintInfoByPtr(StatInfo* info)
{
	cout << "-------�ּ� ���� ��� -------" << endl;
	cout << "HP : " << info->hp << endl;
	cout << "ATT : " << info->attack << endl;
	cout << "DEF : " << info->defence << endl;
	cout << "---------------------------" << endl;
}
#pragma endregion

// 3) ���� ���� ���
void PrintInfoByRef(StatInfo& info)
{
	cout << "-------���� ���� ��� -------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.attack << endl;
	cout << "DEF : " << info.defence << endl;
	cout << "---------------------------" << endl;
}

// �� ����, �ּ� ����, �׸��� �� 3�� �ɼ� ���� ����

int main()
{
	// ������ ����
	StatInfo info;
	CreateMonster(&info);
	PrintInfoByCopy(info);
	PrintInfoByPtr(&info);

	PrintInfoByRef(info);
	// ���� ����


	// �ٽ� ����ڸ� ���� �ڵ�� number��� �̸��� �ٱ��Ͽ� 1�̶�� ���� �־���� ��
	// ���� number���� �� �ְų� �������� �ϸ� �������� �˾Ƶ�� number�� �ּ�(.data, .stack, .heap)�� ���� �����Ѵ�!
	int number = 1;

	// * �ּҸ� ��� �ٱ���
	// type* ���� ���̸� type�� ��� ������ �ٱ��ϸ� �����
	// *�������̸� ���� ���̸� ������ �ּҰ� ����Ű�� ������ ��~ ����!
	// &�����̸� �� ������ �ּҸ� ����
	int* pointer = &number;
	*pointer = 2;

	// ������ ǥ����(&)
	// �����Ϳ� ����� ���������� *��� &�� ���
	// �ο췹��(�����) �������� ���� �۵� ����� int* �� �Ȱ���
	// ������ c++���� ��뼺�� �Ϲ� ������ ������!
	int& reference = number; 

	// C++ ���������� number��� �ٱ��Ͽ� �� �ٸ� �̸��� �ο��Ѱ�
	// number��� �ٱ��Ͽ� reference��� �ٸ� �̸��� �����ٰ�~
	// ������ reference �ٱ��Ͽ� �� �����ų� ������
	// ���� number �ٱ���(�������ٰ�) �� ���� �����ų� ������ ��!

	reference = 3;

	// �׷��� ������ �� �ٸ� �̸��� ��������?
	// �׳� number = 3; �̶�� �ص� �Ȱ�����...
	// �� ������ �Լ����� �Ű������� ������ �� �� ���ϰ� �ϱ� ����


	return 0;
}