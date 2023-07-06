#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ClientPacketHandler.h"

char sendData[] = "Hello World!";

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
	}
	
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		// 이제는 OnRecvPacket이 굳이 처리한 패킷의 크기를 반환할 필요가 없습니다. 
		// 이미 OnRecv에서 패킷이 온전히 온걸 보장하기 때문입니다. 

		// 저번 시간에 패킷을 처리하던 코드를 모두 아래쪽에서 처리합니다.
		ClientPacketHandler::HandlePacket(buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
	}

	virtual void OnDisconnected() override
	{
	}
};


int main()
{
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
