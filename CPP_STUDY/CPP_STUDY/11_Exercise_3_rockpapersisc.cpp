#include <iostream>
using namespace std;

// ���� ���� �� ���α׷�

// ����) ������ �� �����ϴ¹�
// srand()��� �Լ��� �õ尪�� �����ϰ� 
// rand() �Լ��� �õ忡 ���� ������ �����մϴ�.
// srand(time()) ���� ���� ���� �ð��� �μ��� �õ带 �����մϴ�.

int main()
{
	srand(time(0));

//	rand();  // 0 ~ 32767 ������ ���� ���

	// ���� ���� �� ���α׷������� 1,2,3 �� ���� �ʿ�
	//int value = 1 + (rand() % 3);		// 3���� ���� �������� 0, 1, 2 ������ �ϳ��� ���Դϴ�.

	const int SCISSORS = 1;
	const int ROCK = 2;
	const int PAPER = 3;

	// �߰� �������� ���������� �·��� ���ϰ� �ͽ��ϴ�.
	int win = 0;
	int round = 0;

	
	while (true)
	{
		cout << "����(1) ����(2) ��(3) �� �ϳ��� ����ּ���!" << endl;
		cout << ">";

		if (round == 0)
		{
			cout << "�·� : ����" << endl;
		}
		else
		{
			// ù �·� ���ϱ� �õ������� ����� �������� �ʰ� 0�� ��µǾ����ϴ�.
			//int winPer = (win / round) * 100;

			// �����⸦ �� �� ���� ���׿��� ���� ������ ������  1/4 �� 0.25���� ������ �ٷ�� ������ 0�� �˴ϴ�
			// �׷��� win / round * 100 �� �պκп� �Ҽ����� ���� ������ 
			// (win * 100) / round �� �ذ� �� �� �־����ϴ�.
			int winPer = (win * 100) / round;
			cout << "�·� : " << winPer << endl;

			
		}
		int computerVlaue = 1 + (rand() % 3);

		int input;
		cin >> input;


		// �� ���� ���� �� ����� ���� ������ win�� round�� 1�� �߰��߽��ϴ�.
		if (input == SCISSORS)
		{
			switch (computerVlaue)
			{
			case SCISSORS:
				cout << "����(�����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case ROCK:
				round++;
				cout << "����(�����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case PAPER:
				win++;
				round++;
				cout << "����(�����) vs ��(��ǻ��) �̰���ϴ�!" << endl;

				break;
			}
		}
		else if (input == ROCK)
		{

			switch (computerVlaue)
			{
			case SCISSORS:
				win++;
				round++;
				cout << "����(�����) vs ����(��ǻ��) �̰���ϴ�!" << endl;
				break;
			case ROCK:
				cout << "����(�����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case PAPER:
				round++;
				cout << "����(�����) vs ��(��ǻ��) �����ϴ�!" << endl;
				break;
			}
		}
		else if (input == PAPER)
		{

			switch (computerVlaue)
			{
			case SCISSORS:
				round++;
				cout << "��(�����) vs ����(��ǻ��) �����ϴ�!" << endl;
				break;
			case ROCK:
				win++;
				round++;
				cout << "��(�����) vs ����(��ǻ��) �̰���ϴ�!" << endl;
				break;
			case PAPER:
				cout << "��(�����) vs ��(��ǻ��) �����ϴ�!" << endl;
				break;
			}
		}
		else
		{
			break;
		}
	}


}