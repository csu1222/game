#include <iostream>
using namespace std;

// ������ ���� : ���� ����


#pragma region ���� 1 ) ���ڿ� ���� ���ϱ�
// ���� 1) strlen �����ϱ�
int StrLenMy(char* str) // ���� ������ �� 
{
	int result = 0;
	while (str[result] != '\0')
	{
		result++;
	}

	return result;
}

int StrLen(char* str)   // ������ ����
{
	int result = 0;
	while (str[result] != '\0')
		result++;

	return result;
}
#pragma endregion

#pragma region ���� 2) ���ڿ� ���� �ϱ�
// strcpy �Լ��� ������ �Լ��� ������ ������ �־ ����ϸ� ������ �ߴµ� ������ �ϴ� ������ �����ϵ��� �ϰڽ��ϴ�.
#pragma warning(disable: 4996)

void StrCpy(char* dest, char* src)
{
	int srclen = StrLen(src);
	for (int i = 0; i < srclen; i++)
	{
		dest[i] = src[i];
	}
	dest[srclen] = '\0';

	// ������ �������� �����ϸ�
	/*char* ret = dest;

	while (*src != '\0')
	{
		*dest = *src;
		
		dest++;
		src++;
	}
	*dest = '\0';
	dest = ret;*/
}
#pragma endregion

#pragma region ���� 3) ���ڿ� ��������
char* StrCat(char* dest, char* src)
{/*  ���� Ǭ Ǯ��
	int i = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	int j = 0;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return dest;*/

	// ������� �˷��ֽ� Ǯ��
	int len = StrLen(dest);
	
	int i = 0;
	while (src[i] != '\0')
	{
		dest[len + i] = src[i];

		i++;
	}
	dest[len + i] = '\0';

	return dest;
}
#pragma endregion

#pragma region ���� 4) ���ڿ� ���ϱ�
int StrCmp(char* a, char* b)
{
	// a�� b��� ���ڿ��� ���ؼ� ASCII �ڵ� �������� ������ 0, ������ ũ�� -1, �������� ũ�� 1�� �����Ѵ�.
	int i = 0;
	while (a[i] != '\0' || b[i] != '\0')
	{
		if (a[i] > b[i])
			return 1;
		else if (a[i] < b[i])
			return -1;	
		i++;
	}
	return 0;
}
#pragma endregion

#pragma region ���� 5) ���ڿ� ������
void ReverseStr(char* str)
{
	/*int len = StrLen(str);	���� ����
	char* cpy = 0;
	StrCpy(cpy, str);

	for (int i = 0; i < len; i++)
	{
		str[i] = cpy[len - i];
	}*/
	// ����
	int len = StrLen(str);

	for (int i = 0; i < len / 2; i++)
	{
		int temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
	}
}
#pragma endregion
int main()
{
	// � ������ ���ڿ��� ���� �𸣴� BUF_SIZE ��� ũ���� ���� �迭�� ����� ���ڽ��ϴ�.
	const int BUF_SIZE = 100;

	char a[BUF_SIZE] = "Hello";
	char b[BUF_SIZE];
	char c[BUF_SIZE] = " World";

	int len = strlen(a);

	int len2 = StrLen(a);

	cout << len << endl;
	

	StrCpy(b, a);

	StrCat(a, c);
	
	char d[BUF_SIZE] = "Hello World";
	int r = StrCmp(a, d);

	ReverseStr(a);

	return 0;
}