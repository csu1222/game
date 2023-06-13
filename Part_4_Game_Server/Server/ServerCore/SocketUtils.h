#pragma once
#include "NetAddress.h"

/*
--------------------
	Socket Utils
--------------------
*/

class SocketUtils
{
public:
	// 각각 비동기 함수의 포인터로 동작할 것임
	// 처음에는 Null 값이지만 런타임에 주소를 긁어와야함
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;
public:
	// WinSock 초기화와 정리를 해줄 함수들 실행은 CoreGlobal 쪽에서 해줄것입니다.
	static void Init();
	static void Clear();

	// 런타임에 위의 포인터 변수들 값을 채워줄 함수
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
	// 소켓을 생성하는 함수 WSASocket 사용
	static SOCKET CreateSocket();

	// 소켓 옵션을 설정하는 함수들 
	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	// Bind, Listen,Close 같은 함수를 래핑해서 사용합니다. 
	static bool Bind(SOCKET socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

// 저번에 소켓의 옵션을 이리 저리 조절하던 함수를 템플릿을 사용하도록 포장합니다. 
template<typename T>
static inline bool SetSocketOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}