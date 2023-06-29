#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

// 이제 Service를 통해 사용할것이기 때문에 날려줍니다. 
//#include "SocketUtils.h"
//
//#include "Listener.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"

/*
Session 클래스의 멤버 함수중 Process류 함수에서 호출하던 오버라이딩용 함수들을 에코서버처럼 동작하게
만들어 봤습니다. 
*/
class GameSession2 : public Session
{
public:
	~GameSession2()
	{
		cout << "~GameSession" << endl;
	}
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		// 받은 데이터를 SendBuffer에 한번은 복사해 들고 있고 
		// Send를 호출할때 마다 이걸 참조하도록 합니다. 
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096);
		sendBuffer->CopyData(buffer, len);
		
		Send(sendBuffer);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

int main()
{	
	// Session 대신 GameSession을 사용했습니다. 
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		 MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	// service를 만들었으면 Start를합니다. 여기서 해주는것은 Listener 객체를 만들고 
	// 그 Listener 의 멤버 함수인 StartAccept를 자기자신을 넘겨주면서 AccpetEx를 호출합니다. 
	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}