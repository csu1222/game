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

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;
	
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	// Select 모델 
	// 이름의 이유는 select 함수가 핵심이 되는 모델이기 때문
	
	vector<WSAEVENT> wsaEvents;
	vector<Session> sessions;
	sessions.reserve(100);

	WSAEVENT listenEvent = ::WSACreateEvent();
	wsaEvents.push_back(listenEvent);
	sessions.push_back(Session{ listenSocket });

	if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
		return 0;


	while (true)
	{
		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], false, WSA_INFINITE, false);
		if (index == WSA_WAIT_FAILED)
			continue;
		// 공식문서를 보면 WSAWaitForMultipleEvents 의 반환값은 이벤트가 동작한 인덱스에
		// WSA_WAIT_EVENT_0 라는 수도 더한 값을 반환한다고 합니다. 
		// 그래서  WSA_WAIT_EVENT_0 를 빼준값이 진짜 반환된 이벤트의 인덱스입니다. 
		index -= WSA_WAIT_EVENT_0;

		// 기본적으로 WSAEventSelect 를 사용하면 이벤트 들이 수동리셋(Manual Reset) 이 기본 상태라고 했습니다. 
		// 지금 한번 이벤트를 켰으니 직접 리셋해줘야 합니다. 
		::WSAResetEvent(wsaEvents[index]);
		// 보통을 그런데 이 다음에 오는 함수인 WSAEnumNetworkEvents 에서 자동으로 인자로 주는 이벤트 객체를 
		// 리셋 시켜 주기 때문에 여기서 직접 수동 리셋을 해주지 않아도 됩니다. 
		
		// 이 시점에서는 아직 어떤 networkEvent 때문에 켜졌는지 알 수 없습니다. 
		// 그래서 그걸 알아보는 함수가 WSAEnumNetworkEvents 함수였습니다. 
		WSANETWORKEVENTS networkEvents;
		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
			continue;

		// 위 함수의 결과물은 세번째 인자인 networkEvents 에 담깁니다. 이 구조체도 조금 복잡하게 되어 있는데
		// long lNetworkEvents , int iErrorCode[FD_MAX_EVENTS] 라는 변수들을 가지고 있습니다. 
		// 이걸 이용해서 에러가 발생했는지, 어떤 네트워크 이벤트가 발생했는지를 체크할 수 있습니다. 
		// 예제로 확인하는게 이해가 쉽습니다. 
		
		// Listener 소켓 체크
		if ((networkEvents.lNetworkEvents & FD_ACCEPT) != 0)
		{
			// Error - Check
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;

			// 여기까지 네트워크 이벤트가 FD_ACCEPT 이고 에러가 나지 않았다면 listenSocket 이 정상적으로 
			// 준비되었다는 말입니다. 이제 accept 로 클라이언트와 연결합니다. 
			// 이번 모델에서는 또 wsaEvents 와 sessions에 각각  클라이언트 소켓과 이벤트 객체를 연동해주어야 합니다. 
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;
				// 처음 listener 소켓을 추가했던 코드를 똑같이 합니다. 
				// 다만 관찰할 네트워크이벤트를 read, write, close 로 합니다. 
				WSAEVENT clientEvent = ::WSACreateEvent();
				wsaEvents.push_back(clientEvent);
				sessions.push_back(Session{ clientSocket });

				if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					return 0;

				// 이제 다음 루프때에는 listenSocket 뒤에 clientSocket 까지 다 관찰하면서 가장 먼저 준비된 소켓의 
				// 이벤트 객체의 시그널을 켜주면 해당 인덱스를 가지고 작업을 시작하게 됩니다. 
			}
		}
		// 위의 FD_ACCEPT 에 걸리는 소켓이 listenSocket 밖에 없다보니 이렇게 특정할 수 있었습니다. 

		// Client Session 소켓 체크 
		// clientEvent 는 read와 write 두가지 모두 체크해야합니다. 
		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
		{
			// Error - Check
			// read, write 각 에러를 체크합니다. 
			if ((networkEvents.lNetworkEvents & FD_READ) && (networkEvents.iErrorCode[FD_READ_BIT] != 0))
				continue;
			if ((networkEvents.lNetworkEvents & FD_WRITE) && (networkEvents.iErrorCode[FD_WRITE_BIT] != 0))
				continue;

			Session& s = sessions[index];

			// Read
			// 이전 select 모델에서 사용했던 방법인 Bytes 수를 가지고 recv할 소켓인지 send 할 소켓인지를 거릅니다.
			if (s.recvBytes == 0)
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
				{
					cout << WSAGetLastError() << endl;
					// TODO : Remove Session
					continue;
				}

				// 실습때 자꾸 recvLen 이 -1 이 뜨길레 찾아보니 소켓 오류코드가 WSAEWOULDBLOCK 이었습니다.
				// WSAEventSelect 모델에서는 가끔 이렇게 WSAEWOULDBLOCK 오류가 나기 때문에 예외처리를 해주었습니다. 
				if (WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				s.recvBytes = recvLen;
				cout << "Recv Data = " << recvLen << endl;
			}

			// Write
			if (s.recvBytes > s.sendBytes)
			{
				// 혹시라도 send에서 데이터를 한번에 다 보내지 못했을 경우를위한 코드
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// TODO : Remove Session
					continue;
				}
				if (WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				// recv 한 만큼 send 했으면 데이터를 다 보낸 것이니 0으로 초기화(미러 서버이기때문)
				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}

				cout << "Send Data = " << sendLen << endl;
			}
		}

		// FD_CLOSE 처리 
		// 만약 서버-클라 간 연결이 끊겼을 경우의 처리 
		if (networkEvents.lNetworkEvents & FD_CLOSE)
		{
			// TODO : Remove Session
		}
	}

	// WinSock 종료
	::WSACleanup();
}