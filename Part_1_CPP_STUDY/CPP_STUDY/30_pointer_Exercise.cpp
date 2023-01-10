#include <iostream>
using namespace std;

// 오늘의 주제 : 연습 문제


#pragma region 문제 1 ) 문자열 길이 구하기
// 문제 1) strlen 구현하기
int StrLenMy(char* str) // 내가 구현한 것 
{
	int result = 0;
	while (str[result] != '\0')
	{
		result++;
	}

	return result;
}

int StrLen(char* str)   // 강의의 내용
{
	int result = 0;
	while (str[result] != '\0')
		result++;

	return result;
}
#pragma endregion

#pragma region 문제 2) 문자열 복사 하기
// strcpy 함수는 오래된 함수고 위험한 구석이 있어서 사용하면 오류가 뜨는데 지금은 일단 오류를 무시하도록 하겠습니다.
#pragma warning(disable: 4996)

void StrCpy(char* dest, char* src)
{
	int srclen = StrLen(src);
	for (int i = 0; i < srclen; i++)
	{
		dest[i] = src[i];
	}
	dest[srclen] = '\0';

	// 포인터 형식으로 구현하면
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

#pragma region 문제 3) 문자열 덧붙히기
char* StrCat(char* dest, char* src)
{/*  내가 푼 풀이
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

	// 강사님이 알려주신 풀이
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

#pragma region 문제 4) 문자열 비교하기
int StrCmp(char* a, char* b)
{
	// a와 b라는 문자열을 비교해서 ASCII 코드 기준으로 같으면 0, 왼쪽이 크면 -1, 오른쪽이 크면 1을 리턴한다.
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

#pragma region 문제 5) 문자열 뒤집기
void ReverseStr(char* str)
{
	/*int len = StrLen(str);	구현 실패
	char* cpy = 0;
	StrCpy(cpy, str);

	for (int i = 0; i < len; i++)
	{
		str[i] = cpy[len - i];
	}*/
	// 정답
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
	// 어떤 길이의 문자열이 올지 모르니 BUF_SIZE 라는 크기의 문자 배열을 만들어 보겠습니다.
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