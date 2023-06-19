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
/*
Listener�� �ƴϸ� Session���� �񵿱� IO �Լ��� ȣ���ϸ鼭 �ڽ��� 
IocpEvent->owner �� �����ٶ� this�� shared_ptr�� �����Ҽ� �ְ� �ϴ� 
enable_shared_from_this<Type> �� ��ӹ޾ҽ��ϴ�. 
*/
class IocpObject : public enable_shared_from_this<IocpObject>
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
	bool Register(IocpObjectRef iocpObject);
	// ��Ŀ �����忡�� �ϰ��� ���� Ž���ϴ� �Լ�
	bool Dispatch(uint32 timeoutMs = INFINITE);
public:
	// IocpCore�� ������ ���� �����Ͱ� �ڽ��� �ڵ� ���Դϴ�. 
	HANDLE _iocpHandle;
};


// TEMP : ServerService�� ����鼭 �ӽ� ������ü GIocpCore�� �������ݴϴ�.
// extern IocpCore GIocpCore;