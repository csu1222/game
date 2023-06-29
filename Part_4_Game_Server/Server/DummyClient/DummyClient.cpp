#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

// TEMP : 임시로 보낼 데이터 버퍼를 만들었습니다. 
char sendData[] = "Hello World!";


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

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096);
		sendBuffer->CopyData(sendData, sizeof(sendData));

		Send(sendBuffer);
	}
	
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		// 1초에 한번 전송하기위함 
		this_thread::sleep_for(1s);

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096);
		sendBuffer->CopyData(buffer, len);

		Send(sendBuffer);
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


int main()
{
	// 서버가 먼저 실행될때 까지 1초를 기다립니다. 
	this_thread::sleep_for(1s);
	
	// ClientService 생성자의 마지막 인자인 최대 동시 접속수를 5개로 늘려서 테스트 해보겠습니다.
	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		5);

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
