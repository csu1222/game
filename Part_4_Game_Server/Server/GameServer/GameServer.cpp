#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "SocketUtils.h"

// 이번 시간에 만든 SocketUtils 를 통해 소켓을 만들어 봅니다. 
int main()
{
	SOCKET socket = SocketUtils::CreateSocket();

	SocketUtils::BindAnyAddress(socket, 7777);

	SocketUtils::Listen(socket);

	// 이번 시간에 만든 AcceptEX를 사용하지 않고 일반 accept를 사용합니다.
	// AcceptEx 는 나중에 iocp 만들때 구현할것입니다. 
	SOCKET clientSocket = ::accept(socket, nullptr, nullptr);

	cout << "Client Connected!" << endl;

	GThreadManager->Join();
}