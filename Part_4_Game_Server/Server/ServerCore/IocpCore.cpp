#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"



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

bool IocpCore::Register(IocpObjectRef iocpObject)
{
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*Key*/0 , 0);

}


bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD		numOfBytes = 0;
	// key ���� ������� �ʰ� �Ǹ鼭 �ǹ̾��� ������ ����� �Ѱ��ݴϴ�. 
	ULONG_PTR	key = 0;
	IocpEvent*	iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// IocpEvent�� ���� �ִ� owner �� �������� �����ؼ� Dispatch���ݴϴ�. 
		IocpObjectRef iocpObject = iocpEvent->owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errorCode = ::WSAGetLastError();
		switch (errorCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// ���⼭�� ���������� IocpEvent->owner �� IocpObjectRef�� �����մϴ�. 
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}
	return true;
}
