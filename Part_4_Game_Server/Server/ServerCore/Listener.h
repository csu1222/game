#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;

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
	// Listener에게 문지기 역할을 해라 라는 함수
	bool StartAccept(NetAddress netAddress);
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

};

