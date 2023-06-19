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
/*
Listener나 아니면 Session에서 비동기 IO 함수를 호출하면서 자신을 
IocpEvent->owner 에 물려줄때 this를 shared_ptr로 참조할수 있게 하는 
enable_shared_from_this<Type> 을 상속받았습니다. 
*/
class IocpObject : public enable_shared_from_this<IocpObject>
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
	bool Register(IocpObjectRef iocpObject);
	// 워커 스레드에서 일감이 없나 탐색하는 함수
	bool Dispatch(uint32 timeoutMs = INFINITE);
public:
	// IocpCore가 가지고 있을 데이터가 자신의 핸들 뿐입니다. 
	HANDLE _iocpHandle;
};


// TEMP : ServerService를 만들면서 임시 전역객체 GIocpCore를 삭제해줍니다.
// extern IocpCore GIocpCore;