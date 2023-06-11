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
};

// 나중에는 다양한 용도의 IO 함수를 호출하게 될겁니다.
enum IO_TYPE
{
	READ,
	WRITE, 
	ACCEPT,
	CONNECT,
};

// 이번 시간에는 Overlapped를 Session이 아닌 따로 구조체에 넣어줄것입니다. 

struct OverlappedEx
{
	WSAOVERLAPPED overlapped = {};
	// 어떤 동작을 할건지 type으로 들고 있게끔 합니다.
	int32 type = 0;		// read, write, accept, connect ... 
};

void WorkerThreadMain(HANDLE iocpHandle)
{
	while (true)
	{
		// GetQueuedCompletionStatus 의 인자들을 만들어주었습니다.
		DWORD bytesTranfered = 0;
		Session* session = nullptr;
		OverlappedEx* overlappedEx = nullptr;

		// iocpHandle 에 해당하는 작업이 처리되고 메인스레드에서 넘겨줬던 데이터들이 인자로 복원이 됩니다.
		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTranfered,
			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);

		// 실패했는지 체크
		if (ret == FALSE || bytesTranfered == 0)
		{
			// TODO : 연결 끊김
			continue;
		}

		// 복원된 overlappedEx 의 type이 정말 READ인지 체크 나중에는 여기서 type에 따라 
		// 처리를 각각 해주면 됨
		ASSERT_CRASH(overlappedEx->type == IO_TYPE::READ);

		// 데이터를 원하는대로 처리
		cout << "Recv Data IOCP = " << bytesTranfered << endl;

		// 주의: 여기서 해당 소켓으로 더 데이터를 수신하려면 WSARecv 코드를 똑같이 실행해줘야 함
		// 두번째 WSARecv 부터는 이 스레드에서 자체적으로 호출해줘야 함


		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = BUFSIZE;

		DWORD recvLen = 0;
		DWORD flags = 0;

		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
	}
}


int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;
	
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
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

	// IOCP (Completion Port) 모델
	// - APC -> Completion Port (쓰레드마다 있는건 아니고 1개, 중앙에서 관리하는 APC큐?)
	// - Alertable Wait -> CP 결과 처리를 GetQueuedCompletionStatus
	// 스레드와 궁합이 굉장히 좋다!

	// IOCP 에서 새로 등장하는 두가지 함수
	// CreateIoCompletionProt
	// GetQueuedCompletionStatus

	// CP 생성
	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// 여러 세션들을 관리해줄 SessionManager 를 vector로 일단 만들어 봅니다.
	vector<Session*> sessionManager;

	// WorkerThread
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
	}

	// Main Thread = Accept 담당
	while (true)
	{
		// accept는 이번에 굳이 논블로킹으로 할 이유가 없기 때문에 블로킹 소켓으로 진행
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
			return 0;


		// 세션을 만들때 이제는 하나만 만드는게 아니라 멀티 스레드 환경에서 만들것이기 때문에 
		// 동적할당으로 Heap 메모리에 만들어 줍니다. 

		Session* session = A_new<Session>();
		session->socket = clientSocket;

		sessionManager.push_back(session);

		cout << "Client Connected !" << endl;

		// 클라 소켓을 CP에 등록
		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, /*Key*/(ULONG_PTR)session, 0);
		// 인자목록 
		// 1) CP에 등록할 소켓을 HANDLE로 캐스팅
		// 2) 위에서 최초에 만든 CP의 핸들
		// 3) 나중에 GetQueuedCompletionStatus 를 할때 구분할수 있는 고유값 (여기서는 session의 주소를 key로 사용)
		// 4) 한번에 사용할 최대 스레드 갯수 (0을 주면 알아서 현재 컴퓨터의 코어갯수만큼을 잡아줌)

		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = BUFSIZE;

		OverlappedEx* overlappedEx = A_new<OverlappedEx>();
		overlappedEx->type = IO_TYPE::READ;

		DWORD recvLen = 0;
		DWORD flags = 0;

		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);

		//::closesocket(session.socket);

		// 유저가 게임 접속 종료! 
		Session* s = sessionManager.back();
		sessionManager.pop_back();
		A_delete<Session>(s);
	}

	GThreadManager->Join();

	// WinSock 종료
	::WSACleanup();
}