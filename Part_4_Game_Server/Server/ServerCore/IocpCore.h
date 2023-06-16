#pragma once

/*
-----------------
	IocpObject
-----------------
*/

/*
Completion Port �� ����� �� �ִ� �����͵��� IocpObject ��� �������ְڽ��ϴ�. 
�� Ŭ������ ��ӹ����� CP�� ��ϵɼ� �ְԵǴ°̴ϴ�. 
������ ����� �������� ���ø� ��� IocpObject �� ������ �Ǵ°��̰� 
��� �Լ��� Dispatch�� ������ IocpEvent ��� ���� Overlapped ����ü�� �Ǵ°̴ϴ�.
*/
class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	// �� �Լ����� CP�� �ϰ����� �����ϰ� �Ǵµ� IocpEvent��� ��ü�� Overlapped ����üó�� ����Ұ��Դϴ�.
	// IocpEvent�� type�� ���� � IO �Լ��� ����Ұ������� �� ���ֽ��ϴ�. 
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;


};

/*
-----------------
	IocpCore
-----------------
*/

/*
* ���� Iocp�� ��ﶧ ��ǻ� �߿��� �Լ��� �ΰ������ϴ�. 
* ::CreateIoCompletionPort()
* ::GetQueuedCompletionStatus() 
* �����ϴ�. �� �� �Լ��� IoccpCore���� �� ������ ����Ұ��Դϴ�. 
*/
class IocpCore
{
public:
	// ������ �Ҹ��� 
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	// ������ IO �ϰ��� Completion Port�� ����ϴ� �Լ� 
	bool Register(class IocpObject* iocpObject);
	// ��Ŀ �����忡�� �ϰ��� ���� Ž���ϴ� �Լ�
	bool Dispatch(uint32 timeoutMs = INFINITE);
public:
	// IocpCore�� ������ ���� �����Ͱ� �ڽ��� �ڵ� ���Դϴ�. 
	HANDLE _iocpHandle;
};

/*
IocpCore�� ����ϴ� �帧�� ���� � �����忡�� Ŭ���̾�Ʈ���� acceptEx�� 
��ġ�� Ŭ�� ������ �޾ƿ��� �� ������ IocpObject Ŭ������ ��ӹ��� 
�� �ٸ� Ŭ���� ���� ��� �����̶�� �սô�. 
������ ���� ���� IocpCore ��ü�� �Ѱ��ָ鼭 Register �Լ��� ����� �ϰ� 
�̷��� �������⼭ ��ϵ� �ϰ����� Worker�����带 ����� Dispatch �Լ��� 
�� �غ�Ƴ� ������ �մϴ�. 
�׷��� �� �غ� �Ǿ timeout �ð� ���� ȣ��Ǵ� �������
IocpObject->Dispatch �� ���� �ʿ��� �۾��� �մϴ�.

�� ������ IocpCore ��ü�� ��� ������������ �����غ����ϴµ� 
�ϴ� ���� ���ǿ����� �ӽ÷� extern ��ü�� �������ݴϴ�.
*/

// TEMP
extern IocpCore GIocpCore;