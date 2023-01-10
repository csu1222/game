#include <iostream>
using namespace std;


// ���� ���� ���� �� ���α׷����� const int �� ����߾����ϴ�.
////const int SCISSORS = 1;
////const int ROCK = 2;
////const int PAPER = 3;

// ����� ����� � ������ �������� ��� ������ �ø� 
// �ѹ��� ������ �ʿ䰡 ����ϴ�.
// �׷� �� '������' enum��ɾ ����մϴ�.
// �ʱ� ���� ���� ���� ���� ������ 0 ���� ���ʴ�� ���� �ο��˴ϴ�.

enum ENUM_SRP  //enum ��ɾ� ���� �̸��� ���� �� �ֽ��ϴ�.
{
	ENUM_SCISSORS = 1,		// ó���� ���� 1�� ���� �Ͽ����� ���� �������� 1, 2, 3 ������ ���� �ο��� ���Դϴ�.
	ENUM_ROCK,
	ENUM_PAPER
};

// const �� enum ���� ���� ����� ������!
// const�� ��� ����� ��Ұ� �Լ� ������ �Լ��� ���� �� �� ���ǿ� ���� �����Ϸ��� 
// - ������ �޸𸮿� ���� �� ���� ���� �ְ� �ƴϸ� �������ϴ� ������ �׳� ������ ġȯ �� ���� �ֽ��ϴ�.
// enum�� ��� ������ �����Ͻ� �ش� ������ ġȯ�Ǽ� ������ �˴ϴ�.
// - �׷��� �ƹ��� enum�� ������ �÷��� �޸𸮿� �����°� �ƴ� �ڵ��� ���������� ���� �� �ֽ��ϴ�.


// #define ��
// - #�� ���� �Ŵ� -> ��ó�� ���ù�
// 1) ��ó��  2) ������ 3) ��ũ
// define�� ��ó�� �ܰ迡�� �ڵ����� ���� ġȯ ���ִ� ��ũ�� ��� �� �� �ֽ��ϴ�.
// - enum ���� ������ enum�� �������� ġȯ �� �� �ְ� ������ �ø��� ��������
// - define�� �����̿��� �͵鵵 ġȯ �� �� �ֽ��ϴ�.
#define DEFINE_SCISSORS 1
#define DEFINE_TEST	cout << "hello world" << endl;


// ����- #include �� 
// - ���ݲ� c++ ������ ���� �� �� #include <iostream> �� �Է� �߾��µ�
// - �׶� #include <���ϸ�> �� ���ϸ� �ش��ϴ� ������ ��°�� �����ؼ� �� ���Ͽ� �����ִ� ����Դϴ�.

int main()
{	
	DEFINE_TEST

	int wins = 0;
	int total = 0;

	srand(time(0));

	while (true) // ���α׷��� ���������� ���� ��Ű�� ���� �Ϻη� ���� ������ �ɾ��ݴϴ�.
	{

		cout << "����(1) ����(2) ��(3) �� �ϳ��� ������" << endl;
		cout << ">";

		if (total == 0)
		{
			cout << "�·� : ����" << endl;
		}
		else
		{
			int winPer = (wins * 100) / total;
			cout << "�·� : " << winPer << endl;
		}

		int input;
		cin >> input;

		int computerValue = 1 + (rand() %3);

		if (input == ENUM_SCISSORS)
		{
			switch (computerValue)
			{
			case ENUM_SCISSORS:
				cout << "����(����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case ENUM_ROCK:
				total++;
				cout << "����(����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case ENUM_PAPER:
				wins++;
				total++;
				cout << "����(����) vs ��(��ǻ��) �̰���ϴ�!" << endl;
				break;
			}
		}
		else if (input == ENUM_ROCK)
		{
			switch (computerValue)
			{
			case ENUM_SCISSORS:
				wins++;
				total++;
				cout << "����(����) vs ����(��ǻ��) �̰���ϴ�!" << endl;
				break;
			case ENUM_ROCK:
				cout << "����(����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case ENUM_PAPER:
				total++;
				cout << "����(����) vs ��(��ǻ��) �����ϴ�!" << endl;
				break;
			}

		}
		else if (input == ENUM_PAPER)
		{
			switch (computerValue)
			{
			case ENUM_SCISSORS:
				total++;
				cout << "��(����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case ENUM_ROCK:
				wins++;
				total++;
				cout << "��(����) vs ����(��ǻ��) �̰���ϴ�!" << endl;
				break;
			case ENUM_PAPER:
				cout << "��(����) vs ��(��ǻ��) �����ϴ�!" << endl;
				break;
			}

		}
		else 
		{
			break;
		}
	}
}