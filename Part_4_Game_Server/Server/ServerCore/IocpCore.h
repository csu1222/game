#pragma once

/*
-----------------
	IocpObject
-----------------
*/

/*
Completion Port 에 등록할 수 있는 데이터들을 IocpObject 라고 관리해주겠습니다. 
이 클래스를 상속받으면 CP에 등록될수 있게되는겁니다. 
이전에 배웠던 내용으로 예시를 들면 IocpObject 가 세션이 되는것이고 
멤버 함수중 Dispatch의 인자중 IocpEvent 라는 것이 Overlapped 구조체가 되는겁니다.
*/
class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	// 이 함수에서 CP의 일감들을 관찰하게 되는데 IocpEvent라는 객체를 Overlapped 구조체처럼 사용할것입니다.
	// IocpEvent의 type을 통해 어떤 IO 함수를 사용할것인지를 알 수있습니다. 
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;


};

/*
-----------------
	IocpCore
-----------------
*/

/*
* 이전 Iocp를 배울때 사실상 중요한 함수는 두개였습니다. 
* ::CreateIoCompletionPort()
* ::GetQueuedCompletionStatus() 
* 였습니다. 이 두 함수를 IoccpCore에서 또 래핑해 사용할것입니다. 
*/
class IocpCore
{
public:
	// 생성자 소멸자 
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	// 소켓의 IO 일감을 Completion Port에 등록하는 함수 
	bool Register(class IocpObject* iocpObject);
	// 워커 스레드에서 일감이 없나 탐색하는 함수
	bool Dispatch(uint32 timeoutMs = INFINITE);
public:
	// IocpCore가 가지고 있을 데이터가 자신의 핸들 뿐입니다. 
	HANDLE _iocpHandle;
};

/*
IocpCore를 사용하는 흐름을 보면 어떤 스레드에서 클라이언트와의 acceptEx를 
마치고 클라 소켓을 받아오면 그 소켓을 IocpObject 클래스를 상속받은 
또 다른 클래스 예를 들어 세션이라고 합시다. 
세션을 만든 다음 IocpCore 객체에 넘겨주면서 Register 함수로 등록을 하고 
이렇게 여기저기서 등록된 일감들을 Worker스레드를 만들어 Dispatch 함수로 
다 준비됐나 관찰을 합니다. 
그러면 잘 준비가 되어서 timeout 시간 내로 호출되는 스레드는
IocpObject->Dispatch 로 각기 필요한 작업을 합니다.

이 내용중 IocpCore 객체를 어디서 관리해줄지를 생각해봐야하는데 
일단 오늘 강의에서는 임시로 extern 객체로 관리해줍니다.
*/

// TEMP
extern IocpCore GIocpCore;