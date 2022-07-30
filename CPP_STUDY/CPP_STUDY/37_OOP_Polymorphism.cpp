#include <iostream>
using namespace std;

// ������ ���� : ���м�(Data hiding)

// ��ü���� (OOP Object Oriented Programming)
// - ��Ӽ�   
// - ���м�  
// - ������  *

/*
 ������ (Polymorphism  =  Poly + morph) = ���� �Ȱ����� ����� �ٸ��� �����Ѵ�
 - �����ε�(Overloading) = �Լ��� �ߺ� ���� = �Լ��� �̸��� ����
 - �������̵�(Overriding) = ������ = �θ�Ŭ������ �Լ��� �ڽ� Ŭ�������� ������
 -- �Լ� MovePlayer(Player* player); �� �θ�ü�� �ڽİ�ü�� ��� �ƿ츦 ���� �־�����
 -- �ڽ� �Լ����� MovePlayer����� �θ�Ŭ������ Move �Լ��� ����Ǿ����ϴ�. �׷��� �������̵� �ϴ� �ǹ̰� ���� ���Դϴ�.
 -- �θ�,�ڽ� �Լ��� �����Լ��� ������ �Ϲ� �Լ������� ���ڷ� ���� Ŭ������ �Լ��� ����Ǿ����ϴ�.
 -- MovePlayer(Player* player); �Լ��� � ���ڰ� �������� ���ǵɶ��� Player* �� ����Լ��� ����ȴٰ� �ߴµ�
 -- �����Լ��� ����ϴ� ���ڷ� ���� Knight�� ����Լ��� ����Ǿ����ϴ�.
 -- ���� MovePlayer �Լ��� Playre�����͸� ���ڷ� �޴µ� Knight�ּҰ� ���ڷ� �����ߴ����� KnightŬ���� ��ü�� PlayerŬ������ ��� �ް� �ֱ� �����Դϴ�.
 -- 


 ���ε�(Binding) = ���´�
 - ���� ���ε�(Static Binding) : ������ ������ ����
 - ���� ���ε�(Dynamic Binding) * �߿� : ���� ������ ����
 
 �Ϲ� �Լ��� ���� ���ε��� �⺻������ ��� 
 ���� ���ε��� ���Ѵٸ�? -> ���� �Լ�(virtual function)  
 �����Լ��� ���´�
 virtual [��ȯŸ��] [�Լ��̸�] [������]

 �׷��� ���� ��ü�� � Ÿ������ ��� �˰� �˾Ƽ� �����Լ��� ȣ�����ذɱ�?
 - ���� �Լ� ���̺� (vftable
 .vftable [ ] 4����Ʈ(32��Ʈ) 8����Ʈ(64��Ʈ) �� �ּ� 
 [VMove] [VDie] 

 ���� ���� �Լ� : ������ ���� '�������̽�'�� �����ϴ� �뵵�� ����ϰ� ���� ���
  - ���� Ŭ������ ������ ���� ���� ���Ǵ� ���� Ŭ�������� �Լ� ����� ������ ����϶�� �ǹ�
  - �߻� Ŭ���� : ���� ���� �Լ��� 1�� �̻� ���ԵǸ� �ٷ� �߻� Ŭ������ ����
    -- ���������� ��ü�� ���� �� ���� ��

 */


class Player
{
public:
	Player()
	{
		_hp = 100;
	}
	void Move() { cout << "Move Player!" << endl; }
	// �����ε� - �޴� ������ ������ Ÿ���� �ٸ��� �̸��� ���� �� �� �ִ�
	//void Move(int a) { cout << "Move Player (int)" << endl; }
	virtual void VMove() { cout << "VMove Player!" << endl; }
	virtual void VDie() { cout << "VDie Player!" << endl; }

	virtual void VAttack() = 0; // ���� ���� �Լ� ����

public:
	int _hp;
};

class Knight : public Player
{
public:
	Knight()
	{
		_stamina = 100;
	}
	void Move() { cout << "Move Knight!" << endl; }

	// �����Լ��� �����Ǹ� �ϴ��� �����Լ���! (������ �� �� virtual�� ���ֵ� �����Լ�)
	virtual void VMove() { cout << "VMove Knight!" << endl; }
	virtual void VDie() { cout << "VDie Knight!" << endl; }
	virtual void VAttack() { cout << "VAttack Knight" << endl; }

public:
	int _stamina;
};

class Mage : public Player
{
public:
	void Move() { cout << "Move Mage!" << endl; }
	virtual void VMove() { cout << "VMove Mage!" << endl; }
	virtual void VDie() { cout << "VDie Mage!" << endl; }
	virtual void VAttack() { cout << "VAttack Mage" << endl; }

public:
	int _mp;
};
void MovePlayer(Player* player)  // * ����� �θ� �ٷ�� �Լ��� ����� ���� ��ü���� �Լ��� ������ �ʾƵ� �ǰڽ��ϴ�.
{
	player->VMove();
	player->VDie();
}

void MoveKnight(Knight* knight)
{
	knight->VMove();
}
int main()
{
	//Player p;		// ���� ���� �Լ��� ���ԵǾ� �߻� Ŭ�������Ǽ� ���������� ��ü�� ������ �� ���������ϴ�.
	//p.Move();
	//MovePlayer(&p);  // �÷��̾�� �÷��̾��? YES
	// MoveKnight(&p);	   // �÷��̾�� ����? NO

	Knight k;
	k.Move();
	MoveKnight(&k);   // ���� ����? YES
	MovePlayer(&k);   // ���� �÷��̾��? YES��

	return 0;
}