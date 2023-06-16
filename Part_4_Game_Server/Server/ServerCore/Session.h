#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/*
--------------------
	Session 선언부
--------------------
*/

/*
접속하는 클라마다 소켓, 주소 등등 연관된 데이터들을 모두 
이 Session으로 관리하게 됩니다. 
그리고 이 Session 도 IocpCore에 등록해 사용합니다. 
*/

// 이번 실습에서는 접속 테스트만 할것이기때문에 간단하게 만듭니다.
class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	/* 정보 관련 함수 */
	void		SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress	GetAddress() { return _netAddress; }
	SOCKET		GetSocket() { return _socket; }

public:
	/* 인터페이스 구현 */
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	// TEMP : 임시로 Recv버퍼를 만듭니다. 나중에는 수정될것입니다.
	char _recvBuffer[1000] = {};

private:
	SOCKET			_socket = INVALID_SOCKET;
	NetAddress		_netAddress = {};
	// 현재 세션의 접속여부를 들고 있습니다. 
	Atomic<bool>	_connected = false;
};

