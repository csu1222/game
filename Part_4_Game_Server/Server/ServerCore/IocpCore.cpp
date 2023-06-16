#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"


// TEMP : 임시로 관리하는 전역객체
IocpCore GIocpCore;

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

bool IocpCore::Register(IocpObject* iocpObject)
{
	/*
	일감을 등록할때도 CreateIoCompletionPort를 사용했고 인자목록은 이렇습니다. 
	1) 일감을 주는 소켓
	2) iocp 핸들
	3) 일감마다의 고유 Key값
	4) CP의 일감을 동시 처리할 최대 스레드 수입니다. 따로 지정해주거나 아니면 0을 주면 알아서 가능한 최대를 정해줍니다.

	여태 배운건 소켓의 일감을 처리하는데만 사용했었는데 사실 다양한곳에 이 함수를 사용할 수 있습니다. 
	그냥 표준 STL Queue도 넣어 줄 수 있고 꼭 네트워크가 아니더라도 다양한 일감을 처리해 줄 수 있기 때문에 
	여기서도 꼭 소켓, 세션만 사용하는게 여기에 등록할 수 있는 것을
	IOCP Object 라는걸로 관리해주겠습니다. 
	*/
	 
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, reinterpret_cast<ULONG_PTR>(iocpObject), 0);

}

// 예전에 Iocp의 문제점으로 CP에 등록된 IocpObject가 외부에서 삭제 되는걸 막기 위해
// 참조 카운트를 도입해야 하지만 오늘은 대략적인 구조만 잡을것이기 때문에 생략합니다.
bool IocpCore::Dispatch(uint32 timeoutMs)
{
	// GetQueuedCompletionStatus에 필요한 인자들
	DWORD		numOfBytes = 0;
	IocpObject* iocpObject = nullptr;
	IocpEvent*	iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT reinterpret_cast<PULONG_PTR>(&iocpObject), OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// 여기로 들어오면 GetQueuedCompletionStatus 함수의 반환값이 true, 성공했다는것입니다. 
		// 그러면 numOfBytes, iocpObject, iocpEvent 변수들의 값이 채워질것이고 CP에 들어있던 일감은 완료되었습니다.
		
		// 이후 원하는 대로 데이터를 이리저리 사용하는 함수입니다.
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		// 여기로 들어오면 함수 반환 값이 false라는 뜻이고 그 에러 코드를 체크합니다. 
		// 꼭 함수호출 실패라는것은 아니고 timeoutMs 가 INFINITE 가 아닐경우 도 여기로 들어옵니다.
		int32 errorCode = ::WSAGetLastError();
		switch (errorCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

}
