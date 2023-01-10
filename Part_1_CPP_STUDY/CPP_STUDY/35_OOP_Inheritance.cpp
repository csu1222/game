#include <iostream>
using namespace std;

// ������ ���� : ��Ӽ� (Inheritance)

// ��ü���� (OOP Object Oriented Programming)
// - ��Ӽ�  * 
// - ���м� 
// - ������



struct StatInfo
{
	int hp;
	int attack;
	int defence;
};



// �޸𸮴�
//  [    [ Player ]   ]
//  [     Archer      ]

// ��� (Inheritance) ? �θ� -> �ڽĿ��� ������ �����ִ°�

// ������(N)/�Ҹ���(1)
// - �����ڴ� ź���� ����ؼ� ȣ��Ǵ� �Լ�?
// -- Archer�� �����ϸ� -> Player�� ������? Archer�� ������?
// -- �ַθ��� ����! �׳� �Ѵ� ȣ������!
// -- �׷��� ������/�Ҹ��� ȣ�����? : �θ������, �ڽĻ�����, �ڽļҸ���, �θ�Ҹ��� ������ �����ȴٰ�� �ϴµ�
// -- ��� ��������� ���� �׷��� �ʾҽ��ϴ�.��ü�� �����Ǹ� �켱 ������ �ڽ� �����ڷ� ȣ���̵ǰ� 
// -- �ڽĻ������� ��ó�� ����(�����κ�{}�� �����ϱ���) �� �ٽ� �θ� �����ڷ� ȣ���� �Ǿ����ϴ�.
// -- �Ҹ����� ��쿡�� �� ���α׷��� ������ ������ ���� ������ �����ϸ鼭 Archer�Ҹ��ڰ� ȣ���̵˴ϴ�.
// -- Atrcher�Ҹ��ڰ� ����ǰ� ���� ��ó������(�Լ��� ������ ���� ���μ����� �Ѿ�� ���� ����) ���� 
// -- �θ�Ҹ��ڸ� ȣ���Ͽ� �����ϰ� �ٽ� �ڽ� �Ҹ��ڷ� ���ƿµ� main�� return���ķ� ������ �˴ϴ�.
// - �θ�,�ڽ� �����ڰ� ���ڸ� �޴µ� �ٸ� �����ڸ� ������ 
// -- ��ü�� �����Ҷ� ���ڸ� ������ �⺻������ �θ�����ڴ� �⺻ ������, �ڽ��� ���ڸ� �޴� �����ڰ� ȣ��˴ϴ�.
// -- �θ� ���ڸ� ���� �����ڸ� ȣ�� �ϰ� ������ �ڽĻ�����(���ڸ� �޴�) ���� : �θ������(����) �� ���·� �ۼ����ָ�˴ϴ�.




// ����� �и� ������ class Player�� ��������ϴ�.
class Player
{
public:
	Player()
	{
		_hp = 0;
		_attack = 0;
		_defence = 0;
		cout << "Player() �⺻ ������ ȣ��" << endl;
	}
	// �θ� �����ڰ� �ϳ��� �ƴҼ� �ֽ��ϴ�.
	Player(int hp)
	{
		_hp = hp;
		_attack = 0;
		_defence = 0;
		cout << "Player(hp) ������ ȣ��" << endl;
	}

	~Player()
	{
		cout << "Player() �Ҹ��� ȣ��" << endl;
	}

	void Move() { cout << "Player Move ȣ��" << endl; }
	void Attack() { cout << "Player Attack ȣ��" << endl; }
	void Die() { cout << "Player Die ȣ��" << endl; }

public:
	int _hp;
	int _attack;
	int _defence;
};

// class �� ������ ���赵
// ���ӿ� �ϳ��� ������ �ִ°� �ƴϰ� ��ġ�� �Ӽ�, ��ɵ��� ���� ���Դϴ�.
// �Ӽ��� ��� ������ ��� struct�������� ������ �� ���� ���Դϴ�.
// ������ ����Լ��� ���� �Լ��̴ϱ� ���ݱ��� ���ٷδ� �� ���� �������ϴ�.
// �̷� ����� �ذ� �� �� ������� ���°��� '���'�� �����Դϴ�.
class Knight
{
public:
	void Move() {}
	void Attack() {}
	void Die() {}
public:
	StatInfo _statInfo;
};

class Mage
{
public:
	void Move() {}
	void Attack() {}
	void Die() {}
public:
	StatInfo _statInfo;
};

// ��� ���� : �θ� Ŭ������ �Ӽ�, ��ɵ��� �����޴°�
// Archer Ŭ������ �� ��� ������ �θ� Ŭ������ �Ӽ����� ��� ���� �޾ҽ��ϴ�.
class Archer : public Player
{
public:
	Archer()
	{
		_agility = 50;
		cout << "Archer() �⺻ ������ ȣ��" << endl;
	}
	Archer(int ahility) : Player(100) // : Player(100) �κ��� �θ�������� ���ڸ� �޴� �����ڸ� ���� ȣ���� �� ���
	{
		_agility = ahility;
		cout << "Archer(agility) ������ ȣ��" << endl;
	}
	~Archer()
	{
		cout << "Archer() �Ҹ��� ȣ��" << endl;
	}

	// ������ : �θ���� ������ �ź��ϰ� ���ο� �̸����� ����?
	void Move() { cout << "Archer Move ȣ��" << endl; }

public:
	// Archer ���� �Ӽ��� �߰� �غ��ڽ��ϴ�.
	int _agility;

};

int main()
{
	Archer A;

	
	A._hp = 100;
	A._attack = 10;
	A._defence = 5;
	A._agility = 50;

	// ���ڸ� �޴� �����ڸ� �ڽ� �θ� �Ѵ� ������ ������
	Archer Robin(100);

	//A.Attack();
	//A.Die();
	//A.Move();
	//A.Player::Move();  // ������ �ߴ� �θ�Ŭ������ ���� �Ӽ��� �������¹�


	return 0;
}