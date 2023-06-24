#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

// TEMP : 임시로 보낼 데이터 버퍼를 만들었습니다. 
char sendBuffer[] = "Hello World!";


class ServerSession : public Session
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		// 연결할때 Send를 한번 합니다. 
		cout << "Connected To Server" << endl;
		Send((BYTE*)sendBuffer, sizeof(sendBuffer));
	}
	
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		// 1초에 한번 전송하기위함 
		this_thread::sleep_for(1s);

		Send((BYTE*)sendBuffer, sizeof(sendBuffer));
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		cout << "Disconnected" << endl;
	}
};

/*
ServerSession 코드를 위와 같이 만들고 난 후 흐름이 어떻게 되냐면 
클라쪽에서 Connect, 서버쪽에서 Accept를 하고 난 후 
클라쪽 Connect와 동시에 "Hello World!" 를 Send 합니다. 이후 서버쪽에서 
OnRecv에서 패킷을 받은후 그대로 다시 Send로 보냅니다. 
클라의 OnRecv도 마찬가지로 Recv,Send 를 하면서 서버 클라간 패킷을 주고받게됩니다.
*/


int main()
{
	// 서버가 먼저 실행될때 까지 1초를 기다립니다. 
	this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1);

	ASSERT_CRASH(service->Start());


	for (int32 i = 0; i < 2; i++)
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
