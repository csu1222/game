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
	// 최초로 Completion Port를 여기서 생성할겁니다. CreateIoCompletionPort 사용
	// 생성할때는 인자를 다 0으로 넣어줬습니다. 
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
	// 혹시 모르니 이상이 있으면 크래시
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	// IOCP 핸들을 닫아줍니다. 
	::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObjectRef iocpObject)
{
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*Key*/0 , 0);

}


bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD		numOfBytes = 0;
	// key 값을 사용하지 않게 되면서 의미없는 변수를 만들어 넘겨줍니다. 
	ULONG_PTR	key = 0;
	IocpEvent*	iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// IocpEvent가 물고 있는 owner 가 누군지를 추출해서 Dispatch해줍니다. 
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
			// 여기서도 마찬가지로 IocpEvent->owner 로 IocpObjectRef를 복원합니다. 
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}
	return true;
}
