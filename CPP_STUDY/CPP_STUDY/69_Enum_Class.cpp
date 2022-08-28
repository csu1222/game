#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>



// ������ ���� : enum class

/*
	C++11�� ���� �߰��� ������ enum class (scoped enum)
	������ ��� enum�� ������(enumerated type, enumeration)�̶�� �ڷ����Դϴ�.

	enum�� enum class�� ���� ������� �־ ��Ȳ�� ���� ���ٰ� �մϴ�. ������ ������ ���� �˾ƺ��ڽ��ϴ�.

	enum class
	1) �̸����� ���� (scoped)
	- �̸����� ��ȿ������ �߰�ȣ ������ �����Ǿ� �ٸ� ������ �ٽ� �̸��� ������ ������ �Ǿ����ϴ�.

	2) �Ϲ��� ��ȯ ����
	- enum class�� �����͵��� enum�� �ٸ��� �Ϻ��� ��ȯ�� �����ְ� ���� ��ȯ�ؼ� ����Ϸ��� ��������� ĳ������ ����� �մϴ�.
	�̷� Ư¡�� ������ enum���� enum class�� �� ���ٴ� ���� �ƴմϴ�. ������ �� ������ ���� �ֽ��ϴ�.
	enum class�� �� �����͵��� int���� �ɷ� �νĵǴ°��� �ƴ� �׳� enum class Ÿ������ �νĵǱ� ������ ���� Text RPG�� input���� 
	���ϰų� �ϴ� �͵��� �ϱ����ؼ��� ������ ĳ������ ������մϴ�.

	enum (unscoped enum(��������))
	1) �̸����� ����
	- ���� enum�� �߰�ȣ�ȿ� ������ �����͵��� ��Ƴ��������� ��ȿ������ �߰�ȣ �ȿ��� �����Ȱ��� �ƴ� ��ǻ� ���������� ���˴ϴ�.
	  ���� �ǽ����� ������ Knight�տ� PT_Knightó�� PT�� �ٿ��������� enum���� �̸��� ������� �ٸ� ������ ������ �Ұ��������ϴ�.

	2) �Ϲ��� ��ȯ����
	- enum�� �����͵��� ��� enumŸ���� ���ڵ�� �Ȱ����� ���ǻ� �̸��� �����ذ��̱� ������ �ٸ� ����Ÿ�Կ� �����ϰԵǸ� �Ϲ������� ��ȯ�� �˴ϴ�.


	��� : enum�̴� enum class�� ���� ������� �ֽ��ϴ�. 8 �� 2�� enum�� �� ���� ���� �̸��� �Ű�Ἥ �����ִ°� �����Ѵٰ� �մϴ�. ������ 
		enum class�� ����ϴ� �е鵵 ��ôϱ� ��� ����ϴ��� �˰�� �־�� �ڽ��ϴ�.
	*/

// unscoped enum  (��������)
enum PlayerType : char  // �⺻�� int�������� �̷��� Ÿ���� �������ټ� �ֽ��ϴ�.
{
	PT_Knight,
	PT_Archer,
	PT_Mage,
};

// scoped enum
enum class ObjectType
{
	Player,
	Monster,
	Projectile,
};

int main()
{
	int input;
	cin >> input;

	// enum	- int�� �񱳰� ����
	if (input == PT_Knight)
	{

	}
	
	// enum class - int�� ���Ϸ��� ĳ���� �ʿ�
	if (input == static_cast<int>(ObjectType::Player));
	{

	}

	return 0;
}