#include <iostream>
using namespace std;

// ������ ���� : ���м�(Data hiding)

// ��ü���� (OOP Object Oriented Programming)
// - ��Ӽ�   
// - ���м�  *
// - ������

/*
 ���м�(Data Hiding) = ĸ��ȭ(Encapsulation)
 ���� �Ǵ°��� ����ϰ� ����ڴ�!
 ����� ����? (������Ȳ������ ���� ����)
 1)	���� �����ϰ� �ǵ帮�� �ȵǴ� ���
 2) �ٸ� ��η� �����ϱ� ���ϴ� ���

 ������ Ű���� public(������), protected(��ȣ�޴�), private(��������)
 �� Ű������� '(���) ���� ������' ��� �մϴ�.
 public : �������׳� ���� �ϰڴ�.  ���� ����ϼ���~~
 protected : ���� �ڼյ����׸� ���
 private : ���� ����Ҳ�! << class Car ���ο����� ��밡��


 ����Ҷ� �ٴ� public�� '��� ����������' �Դϴ�.
 ��� ���� ������ : ���� ���뿡�� �θ���� ������ ��� ��������?
 �θ������ �������� ������ �� �ڽĵ鿡�Ե� �Ȱ��� ������� ���� ����
 public : (��κ��� ��Ȳ���� ���ϰ�) �θ���� ������ ���� �״�� ������ (public->public, protected->protected)
 protected : �� �ڼյ鿡�Ը� ������ �� (public->protected, protected->protected)
 private : �������� �� ���� �ڼյ鿡�Դ� �� �����ٰž� (public->private, protected->private)
 (����) ��� ���� �����ڸ� ��԰� �ۼ� ���ϸ� private�� �����˴ϴ�.
 */


// 1) �����ϰ� �ǵ帮�� �ȵǴ� ���
#pragma region ���� �ڵ���
// ���÷� �ڵ���
// - �ڵ�
// - �д�
// - ����
// - ��
// - ���� ���⼱

// �Ϲ� ������ ���忡�� ����ϴ� ��?
// - �ڵ�/ �д�/ ��
// ���� �Ǵ� �κ�(������ �ǵ帮�� ū�ϳ�)
// - ����/ ���� ���⼱
#pragma endregion

class Car
{
public:
	void MoveHandle() {	}
	void PushPedal() { }
	void OpenDoor() { }

	void Turnkey()
	{
		// ... 
		RunEngine();
	}

//private:  // 
protected:  // �ڼյ� ���ο��� ���� ��밡��
	void DisassembleCar() { } // ���� �����Ѵ�
	void RunEngine() { } // ������ ������Ų��
	void ConnectCircuit() { } // ���⼱ ����

public:
	// �ڵ�
	// �д�
	// ����
	// ��
	// ���� ���⼱
};

class SuperCar : public Car // ��� ���� ������
{
public:
	void PushRemoteController()
	{
		RunEngine();
	}
};


// 2) �ٸ���η� �����ϱ� ���ϴ� ���
// 'ĸ��ȭ'
// - ������ �����Ϳ� �Լ��� �������� ������� ��

class Berserker
{
public:
	// ��� : ü���� 50 ���Ϸ� �������� ����Ŀ ��� �ߵ�
	int GetHp() { return _hp; }
	void SetHp(int hp)
	{
		_hp = hp;
		if (hp < 50)
			SetBerserkerMode();
	}

private:
	void SetBerserkerMode()
	{
		cout << "�ſ� ������" << endl;
	}

private:
	int _hp = 100;
};


int main()
{
	Car car;
	
	Berserker b;

	b.SetHp(100);
	


	return 0;
}
