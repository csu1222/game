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
	
	vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true)
	{
		// 소켓 셋 초기화
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		// ListenSocket 등록
		FD_SET(listenSocket, &reads);

		// 소켓 등록
		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		// select의 첫번째 인자는 select 함수가 리눅스와 인자 갯수를 맞춰주기 위함 윈도우에서는 0을 넣어주면됨
		// 마지막 인자는 옵션인자인데 timeout(대기시간) 인자 설정 가능 지금처럼 nullptr을 넣어주면 한없이 소켓이 준비될때 까지 대기

		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			break;
		
		// select 를 통과했다는 이야기는 준비된 소켓만 남고 나머지는 다 제거되어있다는 이야기

		// Listener 소켓 체크
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connencted!" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}
		// 여기까지 첫번째 루프에서는 listenSocket 만 있었는데 이제 부터는 접속한 클라이언트 세션도 같이 들어옵니다.

		// 나머지 소켓 체크
		for (Session& s : sessions)
		{
			// reads 체크
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					// 연결이 끊긴 상태
					// TODO : sessions 에서 제거
					continue;
				}

				s.recvBytes = recvLen;
			}

			// writes 체크
			if (FD_ISSET(s.socket, &writes))
			{
				// 블로킹 모드 -> 모든 데이터 다 보냄
				// 논블로킹 모드 -> 일부만 보낼 수가 있음 (상대방 수신버퍼 상황에 따라)
				// 밑의 send 인자를 준이유가 논블로킹 모드에서 이전 시도에 온전히 데이터를 보내지 못했을 수도 있어서
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen <= 0)
				{
					// 연결끊김
					// TODO : Sessions 에서 제거
					continue;
				}

				// 받은만큼(recvBytes) 보냈으면(sendLen) 정상적으로 동작했으니 다시 처음부터 시작
				s.sendBytes += sendLen;
				if (s.sendBytes == s.recvBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}
			}
		}
	}

	// WinSock 종료
	::WSACleanup();
}