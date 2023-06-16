#include "pch.h"
#include "IocpEvent.h"


/*
----------------
	IocpEvent
----------------
*/
IocpEvent::IocpEvent(EventType type) : _type(type)
{
	Init();
}

void IocpEvent::Init()
{
	// OVERLAPPED 구조체 안이 값들을 모두 0으로 밀어줍니다. 
	// 운영체제에서 사용할 값이기 때문이 이후로는 신경쓰지 않을겁니다. 
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
