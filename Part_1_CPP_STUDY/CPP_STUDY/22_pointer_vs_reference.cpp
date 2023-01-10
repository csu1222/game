#include <iostream>
using namespace std;

// ������ ���� : ������ vs ����

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

StatInfo globalInfo;

void PrintInfo(StatInfo* info)
{
	if (info == nullptr)  // nullptr ���� ó��
		return;

	cout << "-------�ּ� ���� ��� -------" << endl;
	cout << "HP : " << info->hp << endl;
	cout << "ATT : " << info->attack << endl;
	cout << "DEF : " << info->defence << endl;
	cout << "---------------------------" << endl;

	// �� �ڿ� �����ٸ�?
	// - �Ű������� (StatInfo* const info)
	// info��� �ٱ����� ���빰(�ּ�)�� �ٲ� �� ���� (�ּҰ� ����!)
	// �� �տ� �����ٸ�?
	// - �Ű������� (const StatInfo* info) or (StatInfo const* info)
	// info�� '����Ű��' �ִ� �ٱ����� ���빰�� �ٲ� �� ����.
	// '����' �ٱ����� ���빰�� �ٲ� �� ����.

	// �� const�� �� �� ��� �� ���� �ִ� (const StatInfo* const info)

	// info[ �ּҰ� ] �� �����ϴ� �Ͱ�
	//info = &globalInfo;
	// �ּҰ�[ ������ ] �� �����ϴ°�
	//info->hp = 1000;
}


void PrintInfo(const StatInfo& info)
{
	cout << "-------���� ���� ��� -------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.attack << endl;
	cout << "DEF : " << info.defence << endl;
	cout << "---------------------------" << endl;

	// �Ű������� const�� �������ν� ������ ������ �����ϴ� �Ǽ��� ����
	// info.hp = 1000; (����)
}

// nullptr�� �뵵 
// Ư�� ������ �����ϴ� ���͸� ã�� �Լ�
StatInfo* FindMonster()
{
	// TODO : Heap �������� ������ ã�ƺ�
	// ã�Ҵ�!
	// return monster~;

	// ��ã�Ҵٸ�?
	return nullptr;   
 }


// OUT ���� ������ ǥ�ù�
#define OUT		// OUT�� �ٸ� �ǹ̸� �ο����� �ʰ� �׳� ��ø��ؼ� ������ �ǹ̷� ���ϴ�.
void ChangeInfo(OUT StatInfo& info)
{
	info.hp = 1000;
}

int main()
{
	// ������ vs ���� ������ ���!
	// ���� : �Ȱ���!
	// ���Ǽ� : ���� ��!

	// 1) ���Ǽ� ���� 
	// ���Ǽ��� ���ٴ°� �� ���� �͸��� �ƴϴ�.
	// �����ʹ� �ּҸ� �ѱ�� Ȯ���ϰ� ������ �ѱ�ٴ� ��Ʈ�� �� �� �ִµ�.
	// ������ �ڿ������� �𸣰� ����ĥ �� ����! (�׻� �Լ��� ������ �����Ѵٰ� ǥ�ð� �Ǿ� �ִ°� �ƴ�!)

	// �Լ��̸��� ������ ������ ��� �Ű������� &info�� ���� �̰� �����ͷ� ������ �ǵ帮�� ���� 
	// �ٷ� �� �� �ֽ��ϴ�.

	// ������ ����� ��� �Լ��̸��� �Ű������� ���¿��� �� �Լ��� ������ ������ �ǵ帮�� ������ �� �� �����ϴ�.
	// �����̳� ����Ʈ ��� ���̴� �Լ��� ������ �ǵ帱 �ʿ䰡 ��� ���� �ް����ϴ�.
	// �� �ڵ尡 ������ �Ѽ��� ���ɼ��� �ִٴ°� �˾������� ����
	// but �׷��� ������� �������°� ���� ����� �ֽ��ϴ�.
	// - const�� ����� ������� �ʰ� �Ѵٴ��� 
	
	// ����� �����Ϳ��� const ��� ����
	// * �������� �տ� ��������, �ڿ� �������Ŀ� ���� �ǹ̰� �޶�����.


	// 2) �ʱ�ȭ ���� 
	// ���� Ÿ���� �ٱ����� 2��° �̸��� �����ִ� ����(��Ī?)
	// -> �ݴ�� �����ϸ� �����ϴ� ����� ������ �ȵ�.
	// �ݸ� �����ʹ� �׳� � �ּҶ�� �ǹ�
	// -> ����� �������� ���� ���� ����.
	// 
	// �����Ϳ��� '����' �� �ǹ̷�? nullptr.   but �ݴ�� ������ Ÿ���� �׻� nullptr Ÿ���� ���ü� �ֱ� ������ 
	// if-else�� üũ�ؼ� ���� ó���� �ؾ� �� ���Դϴ�. (25 ����)
	// ���� Ÿ���� �̷� nullptr�� �����Ƿ� 60���ο� �ִ� nullptr�� Ȱ�� ������ ���ϴ� ������ �ִ�~
	// 
	// �׷��� �����?
	// ��� team by ream... ������ ���� ����
	// ex) ���ۿ��� ���� ���¼ҽ��� ���� ���� ������ ������ ���
	// ex) �𸮾� �������� reference��  �ֿ�
	// 
	// Rookiss ��ȣ ��Ÿ��)
	// - ���� ��쵵 ����ؾ� �Ѵٸ� pointer (null üũ �ʼ�)
	// - �ٲ��� �ʰ� �д� �뵵 (readonly)�� ����ϸ� const ref
	// - �� �� �Ϲ������� ref ��ȣ (��������� ȣ���� �� OUT�� ���δ�.)  (77 ����)
	// -- �� �ٸ������ pointer�� ����� ��Ȳ�̸� pointer�� ��� (���� ������� �ʴ´�)
	// 
	// ex) 
	StatInfo example;
	StatInfo* pointer2 = nullptr;  // or = NULL; or = 0;  : ���� '����' ��� �ǹ� ��� �ּҵ� ����Ű�� ���� �ʴٶ�� �ǹ�
	StatInfo* pointer;	// �����ʹ� ������ ���� ����
	pointer = &example;
	PrintInfo(pointer);

	StatInfo& reference = example;  // ������ ���� �� ����� �־�� ����
	PrintInfo(reference);
	// 

	ChangeInfo(OUT example); // 77 ����


	StatInfo info;
	PrintInfo(&info);
	PrintInfo(info);


	// Bonus) �����ͷ� ����ϴ��� ������ �Ѱ��ַ���?
	// int item = 10;
	// int* ptr = &item;
	// funcByRef(*ptr);    �Ű������� �������� �޴� �Լ��� �����͸� ������ ������ �տ� *�� ���� ���� Ÿ������ ��ȯ�մϴ�.
	// 
	// Bonus) ������ ����ϴ��� �����ͷ� �Ѱ��ַ���?
	// �̰�� �ݴ�� 
	// funcByPtr(&ref);  
	//

	return 0;
}