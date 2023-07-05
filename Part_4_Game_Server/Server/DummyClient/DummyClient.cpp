#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"

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
	
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override
	{
		// BufferReader 도 읽을 버퍼의 영역을 찝어 줍니다. 
		BufferReader br(buffer, len);

		// header 변수에 받은 패킷에 PacketHeader 만큼의 데이터를 넘겨주고 그 만큼 _pos를 땡겨놓습니다.
		PacketHeader header;
		br >> header;

		// 데이터를 읽을때 조심해야하는것이 데이터를 쓴 순서 그대로 읽어야 합니다. 
		// id(uint64), 체력(uint32), 공격력(uint16)
		uint64 id;
		uint32 hp;
		uint16 attack;

		br >> id >> hp >> attack;

		cout << "ID :" << id << " HP :" << hp << " ATT:" << attack << endl;

		// 이제 recvBuffer에 남은 데이터를 복사해줘야 하는데 이 부분이 지금은 지저분합니다. 
		char recvBuffer[4096];
		br.Read(recvBuffer, header.size - sizeof(PacketHeader) - 8 - 4 - 2);
		// 왜 이렇게 지저분 하냐면 이미 받은 패킷에서 PacketHeader와 id, hp, attck 만큼을 읽었기 때문입니다.
		// 당연히 이렇게 처리하는게 옳은 방법은 아니고 나중에 수정할것입니다. 
		// 결론적으로는 이렇게 가변이 되는 데이터의 길이를 따로 보내주는것입니다. 
		// 지금은 일단 이렇게 넘어갑니다.
		
		cout << "Recv Data = " << recvBuffer << endl;

		return len;
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
