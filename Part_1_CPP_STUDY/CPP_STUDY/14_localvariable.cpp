#include <iostream>
using namespace std;

// ������ ���� : ���� ���� �� �� ����

// ���� ������ ���� ���� �ݴ� �Ǵ� ������ ���� ������ ���� �˾ƺ��ڽ��ϴ�.
// ���� ����
// - �Լ� �ۿ� ���ǵ� ����
// - ���������� �� ���� ���� ����������� ������ �� �� �ֽ��ϴ�.
// - const���� �� �ʱ�ȭ ���� �� ���� .rodata, .data, .bss ������ ����˴ϴ�. 

int GloblaVariable = 0;
// �Լ����� ���������� ������ ���� test
void Test()
{
	cout << "���� ������ ���� : " << GloblaVariable << endl;
	GloblaVariable++;
}


// 1) ���� ������ ���� ����
void IncreseHp(int hp)
{
	hp = hp + 1;
}
int main()
{	
	// ���� ����
	// - �Լ� ���� ���ǵ� ����
	// - ���ǵ� �Լ��� ���ÿ� ���� �˴ϴ�.
	// - ���ǵ� �Լ� �������� ��ȿ�� �����Դϴ�.
	int LocalVariable = 1;

#pragma region 1) �������� ���� 
	int hp = 1; 

	cout << "�Լ� ������ hp : " << hp << endl;  // ��� : hp = 1
	IncreseHp(hp);
	cout << "�Լ� ������ hp : " << hp << endl;  // ��� : hp = 1
	// �� �Լ��� ���� hp�� ���� ��µ� ����غ��� ���� ������?
	// main �Լ� �ȿ� ���ǵ� hp ������ ����������� �ϰ�  IncreaseHp(hp)�� �Ű����� hp�� �Ű�������� �� ��
	// �Ű����� hp�� ���������� ���� ���°� �ƴ� �������� hp�� ���� �Ű����� hp�� ���� �� ���Դϴ�.
	// �׷��� IncreseHp �Լ������� ���� ������ ���� �Լ� �������� ��ȿ�ϰ� 
	// main�Լ��� ���� ���� hp�� ������ �ʴ� ���Դϴ�.
	// �׷��� ��� �̹����� �ذ��ؼ� �Լ��� ������ ���� ������?
	// - ���� ��� ������, ������ �̿��ؼ� �ذ� �� �� �ִٰ��մϴ�.
#pragma endregion
}