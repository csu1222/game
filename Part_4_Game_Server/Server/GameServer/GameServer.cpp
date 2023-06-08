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

// 이번 시간에는 WSAOVERLAPPED 구조체를 들고 있을것이고 에코 서버가 아니라 데이터를 받기만 할것
// 이기에 sendBytes 를 지웠습니다. 
struct Session
{
	WSAOVERLAPPED overlapped = {};
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
};

// 각 인자들의 역할 
// 1) 오휴 발생시 0이 아닌 값
// 2) 전송 바이트 수
// 3) 비동기 입출력 함수 호출시 넘겨준 WSAOVERLAPPED 구조체의 주소값
// 4) 사용하지 않을 것이니 0으로 넣어준다
void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
{
	cout << "Data Recv Len Callback = " << recvLen << endl;
	// TODO : 만약 에코서버를 만든다면 여기서 WSASend() 를 호출하면 됨

	Session* session = (Session*)overlapped;
}

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


	//OverLapped 모델(이벤트 기반)
	//1) 비동기 입출력 지원하는 소켓을 생성 + 통지를 받기 위한 이벤트 객체를 생성
	//2) 비동기 입출력 함수 호출(1에서 만든 이벤트 객체를 같이 넘겨줌, 인자중 WSAOVERLAPPED 구조체 안에 hEvent 변수에 넘겨줌)
	//3) 비동기 작업이 바로 완료 되지 않으념, WSA_IO_PENDING 오류 코드가 뜰것
	//- 운영체제는 이벤트 객체를 signaled 상태로 만들어 완료상태를 알려줄것
	//4) WSAWaitForMultipleEvents 함수를 호출해서 이벤트 객체의 signaled 상태를 판별
	//5) WSAGetOverlappedResult 함수를 호출해서 비동기 입출력 결과 확인 및 데이터 처리

	while (true)
	{
		// accept 는 일단 Sync-NonBlocking 방식으로 호출
		SOCKET clientSocket;
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);
		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
				break;
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 문제 있는 상황
			return 0;
		}

		// 세션에 소켓과 이벤트 객체를 담았습니다. 
		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		session.overlapped.hEvent = wsaEvent;

		cout << "Client Connected !" << endl;

		// 클라이언트와 연결이 되었고 클라쪽에서 계속 메세지를 보낼것입니다. 
		while (true)
		{
			// OVERLAPPED 함수가 필요한 인자들을 다 만들어 주었습니다. 
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;

			// 이렇게 호출한 WSARecv는 바로 값을 반환 할 수도 있고 아니면 나중에 반환할 수도 있습니다. 
			// 아마 수신 버퍼에 이미 데이터가 있다면 수신 성공을 하면서 빠져나올것이고 ,
			// 수신 버퍼에 데이터가 없어도 빠져나오기는 할겁니다. 

			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallback)
				== SOCKET_ERROR)
			{
				// 연결 실패일 수도 있고 수신버퍼에 데이터가 없을 수도 있습니다.
				
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					cout << "Pending" << endl;

					// Alertable Wait

					// INFINITE 시간동안 기다리는 Alertable Wait 상태로 만들어 준다는 의미 
					::SleepEx(INFINITE, TRUE);

					// 아래 함수도 마지막 인자를 TRUE로 주면 이 스레드를 Alertable Wait 상태로 변경해줄 수 있습니다.
					// ::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, TRUE);
				}
				else
				{
					// TODO : 문제 있는 상황
					break;
				}
			}
			else
			{
				// 수신 성공
				cout << "Data Recv Len = " << recvLen << endl;
			}
		}

		::closesocket(session.socket);
		::WSACloseEvent(session.overlapped.hEvent);
	}


	// WinSock 종료
	::WSACleanup();
}