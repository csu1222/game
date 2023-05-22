#include "pch.h"
#include <iostream>

// 윈도우즈에서 네트워크 시작하기 위한 라이브러리
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


int main()
{

	/*
	WinSock 초기화 (ws2_32 라이브러리 초기화)

	WinSock2 를 사용하기 위해 먼저 초기화를 해야합니다. 
	WSAStartup 을 공식 문서에서 보면 인자를 두개 받는데 
	WORD  wVersionRequired,  LPWSDATA lpWSAData 를 받습니다. 
	첫번째 인자는 버전을 정하는 건데 1.0, 1.1, 2.0, 2.1, 2.2 가 있다고 하지만 현재 보통
	2.2 버전만을 사용한다고 합니다. 
	관련 정보가 두번째 인자인 wsaData 에 담기기는 하지만 딱히 따로 사용을 하지는 않을겁니다. 
	*/
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;	

	// 참고로 소문자로 되어있는 함수는 리눅스 환경에서도 호환 대문자와 섞여있으면 winSock 에서
	// 사용하는 함수일 확률이 높습니다. 항상 맞는건 아니라고 합니다. 


	// af : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor 
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0); 
	
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	// 이제 서버쪽의 소켓(대표전화번호) 를 받아와야 합니다. 
	SOCKADDR_IN serverAddr;	// IPv4 버전
	::memset(&serverAddr, 0, sizeof(serverAddr));	// serverAddr 일단 0으로 밀어줍니다.
	serverAddr.sin_family = AF_INET;
	// serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); << deprecate
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);	

	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Connect ErrorCode : " << errCode << endl;
		return 0;
	}

	// ------------
	// 연결 성공! 이제부터 데이터 송수신 가능!
	cout << "Connected To Server!" << endl;

	while (true)
	{
		//TODO
		char sendBuffer[100] = "Hello World!";

		for (int i = 0; i < 10; i++)
		{
			int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Send ErrorCode : " << errCode << endl;
				return 0;
			}
		}


		cout << "Send Data! Len : " << sizeof(sendBuffer) << endl;
		
		/*	char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Recive ErrorCode : " << errCode << endl;
				return 0;
			}

			cout << "Recv Data! Data = " << recvBuffer << endl;
			cout << "Recv Data! Len = " << recvLen << endl;*/

		this_thread::sleep_for(1s);

	}
	//-------------

	// 소켓 리소스 반환 
	::closesocket(clientSocket);

	// WinSock 종료
	::WSACleanup();
}
