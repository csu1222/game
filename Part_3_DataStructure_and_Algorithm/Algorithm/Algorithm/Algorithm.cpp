#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include <thread>

// 오늘의 주제 : 해시 테이블 Hash Table

// 테이블
void TestTable()
{
	struct User
	{
		int userId = 0;		// 1 ~ 999
		string userName;
	};

	vector<User> users;
	users.resize(1000);

	// 777 번 유저 정보 세팅
	users[777] = User{ 777, "SeaBass" };

	// 777 번 유저의 이름은 무엇입니까.
	string name = users[777].userName;

	cout << name << endl;
}

// 해시
void TestHash()
{
	struct User
	{
		int userId = 0;		// 1 ~ 999
		string userName;
	};

	vector<User> users;
	users.resize(1000);

	const int userId = 123456789;
	int key = (userId % 1000);  // hash < 고유번호 

	// 123456789 번 유저 정보 세팅
	users[key] = User{ userId, "SeaBass" };

	// 123456789 번 유저의 이름은 무엇입니까.
	User& user = users[key];
	if (user.userId == userId)
	{
		string name = user.userName;
		cout << name << endl;
	}

}

void TestHashTableChaining()
{
	struct User
	{
		int userId = 0;		// 1 ~ 999
		string userName;
	};

	vector<vector<User>> users;
	users.resize(1000);

	const int userId = 123456789;
	int key = (userId % 1000);  // hash < 고유번호 

	// 123456789 번 유저 정보 세팅
	users[key].push_back(User{ userId, "SeaBass" });
	users[789].push_back(User{ 789, "Seonguk" });

	vector<User>& Bucket = users[key];

	for (User& user : Bucket)
	{
		if (user.userId == userId)
		{
			string name = user.userName;
			cout << name << endl;
		}
	} 
}

int main()
{
	vector<int> v;

	srand(static_cast<int>(time(0)));


	for (int i = 0; i < 50; i++)
	{
		int randValue = rand() % 100;

		v.push_back(randValue);
	}

	TestHashTableChaining();

}