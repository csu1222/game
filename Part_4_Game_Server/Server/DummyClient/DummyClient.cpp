#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

char sendData[] = "Hello World!";

// Session 대신 PacketSession을 상속 받습니다.
class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		// cout << "Connected To Server" << endl;

		// Connect때 Send를 하는것 대신 GameServer 에서 뿌려주는 패킷을 받을것입니다. 
	}
	
	// OnRecv 대신 OnRecvPacket
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override
	{
		// GameSession 에서 하던것처럼 받은 패킷의 헤더 내용들을 추출해 출력
		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[0]));

		cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

		// 헤더 외에 받은 내용이 궁금하다면 임시 버퍼를 만들어 줍니다.
		char recvBuffer[4096];
		::memcpy(recvBuffer, &buffer[4], header.size - sizeof(PacketHeader));
		
		cout << "Recv Data = " << recvBuffer << endl;

		return len;
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
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
		1000);

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
