#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
// ServerService 를 사용하기 시작
class ServerService;

/*
--------------------
	Listener 선언부
--------------------
*/
// listenSocket 같은 역할을 하게 될겁니다. 
// 이 클래스도 CP에 등록되어 작업을 할것이기에 IocpObject를 상속받았습니다.
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* 외부에서 사용 */
	// NetAddress 대신 ServerServiceRef 를 인자로 받습니다. 
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();


public:
	// IocpObject를 상속했기 때문에 반드시 정의해줘야하는 함수들 
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

	
protected:
	// 멤버 변수들 
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
	// TODO : 나중에 세션 카운트 같은 데이터도 들고 있을 수 있습니다. 

	// StartAccept 에서 받은 ServerService를 변수로 들고 있을겁니다.
	// 하지만 ServerService 에서도 Listener를 들고 있고 여기서도 ServerService를 들고 있게되면 
	// shared_ptr 로는 서로 순환 문제를 격을 수 있습니다. 이걸 유념해서 구현하겠습니다. 
	ServerServiceRef _service;

};

