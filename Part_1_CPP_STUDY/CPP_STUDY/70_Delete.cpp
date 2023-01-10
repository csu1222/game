#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>


// ������ ���� : delete ( ������ �Լ� )

/*
	�̹��� ��� delete�� �����Ҵ� �ð��� ��� new-delete������ �ƴ� ������ �Լ��� ����� �����Դϴ�.
	nullptr�� �����Ҷ� ��񳪿Ծ��µ� �׶��� private�� operator&�� ���Ƴ����ϴ�. �׷������� ����ϴ� �����Դϴ�.
	������ private�� ���� ������ �ܺο����� ������ �� ������ �ڱ� ���ο����� ������ �� �ִ� ����¥�� �����̾����ϴ�.

	�ؿ� ���÷� ���� Knight Ŭ�������� ���Կ�����(=)�� �������ؼ� private�� �����Ͽ����ϴ�. 
	�׸��� �ǿ������� ������ �������� �н��� ���� Admin�̶�� Ŭ�������� ��� �Լ��� Knight�� ���翬���� (=)�� ����غý��ϴ�.
	
	KnightŬ�������� private�� ���� ������ Admin���� ���翬���ڸ� ����Ҽ� �������� 
	���⼭ �� ���ο� ������ friend �������� frined class Admin�� Knight Ŭ���� ���ο� �Է��ϸ� �ٸ� Ŭ������ private������ �����Ҽ� �ְԵ˴ϴ�.
	�̷��� ��� ���ϰ� �������� = �� ����ϰ� �Ǿ� ���Ƚ��ϴ�.

	�� ���ô� �߻������ �ʴ� ��Ȳ(friend class���� ����ؼ� �����ϴ� ��Ȳ)�� ���÷� ������� �׷��� �ᱹ �ո��� �Ѵٴ� �� �˷��ݴϴ�.
	private�� ������ �� �̻� ������ �Ҽ� �ֽ��ϴ�.

	�׷��� KnightŬ������ void operator= (const Knight& k); �̷��� { } �� �����θ� ������ ������ ����� ����� ������ �����ϸ� ��ũ������ ���ϴ�.
	�̷��� �и� = �������� ����� �������� ������ ����� ����Ǽ� �Ѵ��� �˾ƺ��� ����ϴ�.

	�̷� ������ delete �������� �ذ� �� �� �ְԵ˴ϴ�. 
	delete������ ������ �����մϴ�. ���� ���ø� �ٽ� ���
	public:
		void operator= (const Knight& k) = delete;
	�̷��� �Լ� ������ = delete; �� ���ָ� �˴ϴ�. 
	delete�� ����ϸ� ������ �����Ϸ��� �ȵȴٰ� �̸� �˷��ֱ⵵�ϰ� ���� �޼����� ������ �Լ��� �����Ϸ��� �Ѵٰ� ģ���� �˷��ݴϴ�.


*/


class Knight
{
	// ���� Knight = Knight ���� ���� �����ڸ� ����ʹٸ�
//private:
//	void operator= (const Knight& k);
//		
//	// private�� ���� �Ҽ� �ֵ��� �����ִ� ����
//	friend class Admin;

	// delete �������� ����
public:	// delete�� public���� ������°� ���ʶ�� �մϴ�.
	void operator= (const Knight& k) = delete;
private:
	int _hp = 100;
};

// �ǿ뼺�� ������ ���� ������ : private�� ��� ���°��� �ҿ�����
class Admin
{
public:
	void CopyKnight(const Knight& k)
	{
		Knight k1;

		k1 = k;
	}
};


int main()
{
	Knight k1;

	Knight k2;

	//k1 = k2;

	Admin a1;
	a1.CopyKnight(k1);

	return 0;
}