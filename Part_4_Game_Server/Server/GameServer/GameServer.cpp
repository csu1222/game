#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{

	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;
	
	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	// 옵션을 해석하고 처리할 주체?
	// 소켓 코드 -> SOL_SOCKET
	// IPv4 -> IPPROTO_IP
	// TCP 프로토콜 -> IPPROTO_TCP

	// SO_KEEPALIVE = 주기적으로 연결 상태를 확인할지 여부
	bool enable = true;
	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));

	// S0_LINGER = 지연하다 closesocket() 을 호출하면 소켓 리소스 반환과 더불어 소켓의 연결을 끊는
	// 동작까지도 해줍니다. 이때 커널쪽 버퍼에 데이터가 남아있다고 하면 이 남은 데이터를 마저 보낸다음 
	// 연결을 끊을것인지 아니면 남은 데이터를 다 날릴것인지를 정해줍니다. 
	// 옵션값으로 LINGER 라는 구조체를 받는데 이 구조체안에는 또 두가지 변수로 이루어져있습니다. 
	// u_short타입의 on/off, linger 라는 두가지 변수가 있습니다. 
	// onoff = 0은 바로 연결끊기, 아니면 linger 초만큼 대기 (default 0)
	// linger = 대기 시간 
	// 
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;

	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

	// Half-Close
	// SD_SEND : send 막는다
	// SD_RECIVE : recv 막는다
	// SD_BOTH : 둘 다 막는단
	// ::shutdown(serverSocket, SD_SEND);
	
	// SO_SNDBUF = 송신 버퍼 크기
	// SO_RCVBUF = 수신 버퍼 크기

	int32 sendBufferSize;
	int32 optionLen = sizeof(sendBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLen);

	cout << "송신 버퍼 크기 : " << sendBufferSize << endl;

	int32 recvBufferSize;
	optionLen = sizeof(recvBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, &optionLen);

	cout << "수신 버퍼 크기 : " << recvBufferSize << endl;

	// SO_REUSEADDR
	// IP주소 및 port 재사용
	{
		bool enable = true;
		::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
	}

	// IPPROTO_TCP
	// TCP_NODELAY = Nagle 네이글 알고리즘 작동 여부
	// 데이터가 충분히 크면 보내고, 그렇지 않으면 데이터가 충분히 쌓일때까지 대기!
	// 장점 : 작은 패킷이 불필요하게 많이 생성되는 일을 방지
	// 단점 : 반응 시간 손해
	{
		bool enable = true;
		::setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
	}
	
	// 소켓 리소스 반환
	::closesocket(serverSocket);
	// WinSock 종료
	::WSACleanup();
}