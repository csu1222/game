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
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	WSAOVERLAPPED overlapped = {};
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

	
	// Overlapped IO (비동기 + 논블로킹)
	// - Overlapped 함수를 건다 (WSARecv, WSASend)
	// - Overlapped 함수가 성공했는지 확인 후 
	// -> 성공했으면 결과 얻어서 처리
	// -> 실패했으면 사유를 확인 사유가 Pending(보류)이라면 나중에 완료되면 알려달라고 두가지 방법중 하나를 선택

	// Overlapped 함수들 
	// WSASend
	// WSARecv
	// AcceptEx		- AcceptEx와 ConnectEx는 나중에 네트워크 라이브러리제작때 자세히 알아볼것
	// ConnectEx

	// WSASend, WSARecv 두 함수가 받는 인자들은 거의 비슷합니다. 어떤 인자를 받는지 알아보겠습니다.
	// 1) 비동기 입출력 소켓
	// 2) 입출력 버퍼를 WSABUF 라는 구조체로 받아줌배열의 시작 주소
	// 3) WSABUF의 크기,개수
	// 4) 보내고/ 받은 바이트 수
	// 5) 상세 옵션인데 일단 0으로 넣으면 됩니다.
	// 6) WSAOVERLAPPED의 구조체의 주소값 
	// 7) 입출력이 완료되면 OS가 호출할 콜백함수 (오늘 실습에서는 사용하지 않을것)

		// WSABUF 에는 buf와 len데이터가 있지만 인자로 따로 배열 시작 주소와 개수를 넣어주는 이유는
		// 밑과 같이 여러버퍼를 한번에 넣어 줄 수 있기때문 
		//char sendBuffer[100];
		//WSABUF wsaBuf[2];
		//wsaBuf[0].buf = sendBuffer;
		//wsaBuf[0].len = 100;

		//char sendBuffer2[100];
		//wsaBuf[1].buf = sendBuffer2;
		//wsaBuf[1].len = 100;

		// 위의 방법을 통해 나중에 알아볼 우아한 기법으로 
		// Scatter-Gether 기법 이라고 합니다. 
		// 나중에 데이터 패킷을 보낼때 패킷들이 서로 흩어져 있을건데 그 패킷들을 위처럼 한 버퍼에 
		// 모으고 연결해서 보내고 있습니다. 일단 이내용은 나중에 다시 다룰 내용입니다. 

		// WSAOVERLAPPED 가 어떻게 되어있는지를 따라 들어가보면 OVERLAPPED라는 구조체이고 
		// 내부적으로는 여러 값들이 복잡하게 있는데 대부분은 운영체제에서 알아서 사용하는 값들이고 
		// 우리가 신경써야하는 데이터는 
		// HANDLE hEvent 라는 핸들을 보면 됩니다. 이 변수에 우리가 이벤트 핸들을 넣어주는 것입니다. 
		// 이 OVERLAPPED.hEvent 라는 곳에 생성한 이벤트를 통해 비동기적으로 동작 완료된 것을 통보받을수 있습니다. 

	//이런 인자들을 받고 있고

	//또 처음 OVERLAPPED 함수를 사용할때 잘못 사용할 경우도 있습니다.
	//WSASend, WSARecv는 비동기 함수이기 때문에 호출할 시점과 완료가 된 시점이 같지 않을 수 있다고 했습니다.그래서
	//두번째 인자 WSABUF와 WSAOVERLAPPED에 대한 정보를 넘겨준 다음 확실히 동작이 완료될때 까지 건드리지 않고 가만히 놔둬야합니다.
	//괜히 동기 함수처럼 코드 줄이 지나갔다고 동작이 완료되었겠꺼니 하고 건드렸다가는 나중에 커널이 미뤄둔 OVERLAPPED함수를 실행할때 우리가 의도한 데이터가 아니라
	//엉뚱한 데이터가 송신 / 수신 될 수 있다는 말입니다.

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
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr)
				== SOCKET_ERROR)
			{
				// 연결 실패일 수도 있고 수신버퍼에 데이터가 없을 수도 있습니다.
				
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					cout << "Pending" << endl;
					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, false);
					::WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, FALSE, &flags);
				}
				else
				{
					// TODO : 문제 있는 상황
					break;
				}
			}

			// 수신 성공
			cout << "Data Recv Len = " << recvLen << endl;
		}

		::closesocket(session.socket);
		::WSACloseEvent(session.overlapped.hEvent);
	}


	// WinSock 종료
	::WSACleanup();
}