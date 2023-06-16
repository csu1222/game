#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"


// TEMP : �ӽ÷� �����ϴ� ������ü
IocpCore GIocpCore;

/*
-----------------
	IocpCore
-----------------
*/
IocpCore::IocpCore()
{
	// ���ʷ� Completion Port�� ���⼭ �����Ұ̴ϴ�. CreateIoCompletionPort ���
	// �����Ҷ��� ���ڸ� �� 0���� �־�����ϴ�. 
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
	// Ȥ�� �𸣴� �̻��� ������ ũ����
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	// IOCP �ڵ��� �ݾ��ݴϴ�. 
	::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObject* iocpObject)
{
	/*
	�ϰ��� ����Ҷ��� CreateIoCompletionPort�� ����߰� ���ڸ���� �̷����ϴ�. 
	1) �ϰ��� �ִ� ����
	2) iocp �ڵ�
	3) �ϰ������� ���� Key��
	4) CP�� �ϰ��� ���� ó���� �ִ� ������ ���Դϴ�. ���� �������ְų� �ƴϸ� 0�� �ָ� �˾Ƽ� ������ �ִ븦 �����ݴϴ�.

	���� ���� ������ �ϰ��� ó���ϴµ��� ����߾��µ� ��� �پ��Ѱ��� �� �Լ��� ����� �� �ֽ��ϴ�. 
	�׳� ǥ�� STL Queue�� �־� �� �� �ְ� �� ��Ʈ��ũ�� �ƴϴ��� �پ��� �ϰ��� ó���� �� �� �ֱ� ������ 
	���⼭�� �� ����, ���Ǹ� ����ϴ°� ���⿡ ����� �� �ִ� ����
	IOCP Object ��°ɷ� �������ְڽ��ϴ�. 
	*/
	 
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, reinterpret_cast<ULONG_PTR>(iocpObject), 0);

}

// ������ Iocp�� ���������� CP�� ��ϵ� IocpObject�� �ܺο��� ���� �Ǵ°� ���� ����
// ���� ī��Ʈ�� �����ؾ� ������ ������ �뷫���� ������ �������̱� ������ �����մϴ�.
bool IocpCore::Dispatch(uint32 timeoutMs)
{
	// GetQueuedCompletionStatus�� �ʿ��� ���ڵ�
	DWORD		numOfBytes = 0;
	IocpObject* iocpObject = nullptr;
	IocpEvent*	iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT reinterpret_cast<PULONG_PTR>(&iocpObject), OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// ����� ������ GetQueuedCompletionStatus �Լ��� ��ȯ���� true, �����ߴٴ°��Դϴ�. 
		// �׷��� numOfBytes, iocpObject, iocpEvent �������� ���� ä�������̰� CP�� ����ִ� �ϰ��� �Ϸ�Ǿ����ϴ�.
		
		// ���� ���ϴ� ��� �����͸� �̸����� ����ϴ� �Լ��Դϴ�.
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		// ����� ������ �Լ� ��ȯ ���� false��� ���̰� �� ���� �ڵ带 üũ�մϴ�. 
		// �� �Լ�ȣ�� ���ж�°��� �ƴϰ� timeoutMs �� INFINITE �� �ƴҰ�� �� ����� ���ɴϴ�.
		int32 errorCode = ::WSAGetLastError();
		switch (errorCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : �α� ���
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

}
