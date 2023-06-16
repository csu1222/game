#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"

/*
--------------------
	Session 정의부 
--------------------
*/
Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// TODO : 나중에 들어오는 IocpEvent에 따라 Recv나 Send를 처리해주게 될겁니다.
	// 지금은 접속 테스트만 할것이기때문에 비워둡니다.
}
