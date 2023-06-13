#include "pch.h"
#include "SocketUtils.h"

/*
--------------------
	Socket Utils
--------------------
*/

// 처음에는 모두 nullptr 로 초기화 
// 이후 런타임에 각자의 주소를 넣어주는게 목표 
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	// WinSock을 초기화 하는 과정 

	WSADATA wsaData;
	// 혹시 제대로 시작하지 못할경우 바로 크래시를 내줍니다. 
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT &wsaData) == 0);

	/*
	다음으로 런타임에 주소를 얻어오는 API를 호출해줘야 합니다. 
	사용할 함수의 공식문서를 보면 내용이 나올것입니다. 
	방법은 잠시 더미 소켓을 만들고 그 소켓을 대상으로 몇몇 함수를 호출해주게 됩니다.
	*/
	SOCKET dummySocket = CreateSocket();

	// 인자 목록
	// 1) 더미 소켓
	// 2) 주소를 찾고 싶은 함수
	// 3) 주소를 받을 포인터
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	// 다 사용한 더미 소켓을 닫아줍니다. 
	Close(dummySocket);
}

void SocketUtils::Clear()
{
	// 윈속들을 정리합니다. 원칙상 윈속 호출 수만큼 호출해야하긴 하지만 굳이 그렇게 하지 않아도 된다고 합니다.
	::WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	// 런타임에 멤버 변수들 값을 채워줄 함수 

	DWORD bytes = 0;
	// 넘겨주는 인자들이 긴데 하나하나 집고 넘어가진 않을겁니다. 나중에 필요할때 문서를 찾아보는걸로 하겠습니다.
	// 대신 이 함수의 하는 일을 보면 connectEx, disconnectEx, acceptEx 함수를 런타임에 불러오기 위한 함수입니다.
	// SOCKET_ERROR 가 났는지를 bool 로 반환하고 있습니다.
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	/*
	지금까지 배운 소켓 만드는 함수는 ::socket() 이었는데 
	::WSASocket() 함수로도 소켓을 만들수 있고 이 함수로 만들면 더욱 세분화된 옵션을 정할 수 있습니다. 
	인자목록
	1) AF				: 주소 패밀리 사양 쉽게 말해 IPv4, IPv6 등을 골라주는 옵션
	2) type				: 새 소켓의 형식 사양 TCP, UDP 를 골라주는 옵션
	3) protocol			:	사용할 프로토콜 이전에는 0을 줘서 알아서 TCP를 골라주도록 했는데 
						이번엔 직접 옵션을 TCP로 지정했습니다.
	4) lpProtocolInfo	: 만들 소켓의 특성을 세부적으로 정하는 WSAPROTOCOL_INFO 구조체에 
						대한 포인터를 받습니다. NULL로 줘서 따로 옵션을 건드리지 않았습니다. 
	5) g				: 새 소켓 및 새 소켓 그룹을 만들지 아닐지, 만든다면 ID 발급 기준을 어떻게 할지를 정함
	6) dwFlags			: 추가적인 소켓의 특성으로 지정하는데 사용되는 플래그 이전에는 이것도 0으로 
						따로 정해주지 않았었습니다. 지금 인자로 넣어준 WSA_FLAG_OVERLAPPED 는 
						비동기 IO 함수를 지원하는 소켓의 옵션입니다.

	반환으로 소켓을 반환합니다.
	*/
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;

	return SetSocketOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

// listenSocket 의 소켓 옵션을 ClientSocket에 그대로 적용한다는 의미 
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN)) ;
}

// bind 에 필요한 SOCKADDR_IN 구조체의 내용을 자신이 받을수 있는 아무 주소로 받아서 bind 까지 해줍니다. 
bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<SOCKADDR*>(&myAddress), sizeof(myAddress));
}

// listen 을 해주면서 그 결과가 성공인지 아닌지를 bool타입으로 반환합니다. 
bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

// 인자를 참조로 받아오고 중복해서 closesocket 하지 않도록 체크, 
// 닫은 socket의 값을 INVALID_SOCKET으로 변경해주고 있습니다. 
void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}
