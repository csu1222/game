#include <iostream>
using namespace std;

// �������� : �����, ������


int main()
{	
#pragma region cout, cin
	// ������������
	// ���²� ���� cout�� ��ǲ�� �޴� ���ο� ��ɾ� cin
	//int input;
	//cin >> input;

	//cout << "hello world" << endl;
	// �ǵ��� endl; �� end line ���� �� ���� ������ �����ٷ� �Ѿ�ٴ� �ǹ�
	// cout << "hello world\n";		���� ǥ���ص� ��������ϴ�.
#pragma endregion

#pragma region �����
	// �������� � ������ �Է��ϸ� 
	// N*N���� ���� ������� ���ھ��

	int input;
	cin >> input;

	for (int t = 0; t < input; t++)
	{
		for (int i = 0; i < input; i++)
		{
			cout << "*";
		}
		cout << endl;
	}
#pragma endregion

#pragma region ����� 2
	// �������� � ������ �Է��ϸ� 
	// N���� ���� ��ܽ����� ������ּ���

	int input2;
	cin >> input2;

	for (int t = 0; t < input2; t++)
	{
		for (int i = 0; i < t + 1; i++)
		{
			cout << "*";
		}
		cout << endl;
		
	}
#pragma endregion

#pragma region ����� 3
	// �������� � ������ �Է��ϸ� 
	// N�� ���� �����ؼ� �ٸ��� 1���� �پ��� ���·�!

	int input3;
	cin >> input3;

	for (int t = 0; t < input3; t++)
	{
		for (int i = 0; i < (input3 - t); i++)
		{
			cout << "*";
		}
		cout << endl;

	}
#pragma endregion


}