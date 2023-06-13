#include "pch.h"
#include "SocketUtils.h"

/*
--------------------
	Socket Utils
--------------------
*/

// ó������ ��� nullptr �� �ʱ�ȭ 
// ���� ��Ÿ�ӿ� ������ �ּҸ� �־��ִ°� ��ǥ 
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	// WinSock�� �ʱ�ȭ �ϴ� ���� 

	WSADATA wsaData;
	// Ȥ�� ����� �������� ���Ұ�� �ٷ� ũ���ø� ���ݴϴ�. 
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT &wsaData) == 0);

	/*
	�������� ��Ÿ�ӿ� �ּҸ� ������ API�� ȣ������� �մϴ�. 
	����� �Լ��� ���Ĺ����� ���� ������ ���ð��Դϴ�. 
	����� ��� ���� ������ ����� �� ������ ������� ��� �Լ��� ȣ�����ְ� �˴ϴ�.
	*/
	SOCKET dummySocket = CreateSocket();

	// ���� ���
	// 1) ���� ����
	// 2) �ּҸ� ã�� ���� �Լ�
	// 3) �ּҸ� ���� ������
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	// �� ����� ���� ������ �ݾ��ݴϴ�. 
	Close(dummySocket);
}

void SocketUtils::Clear()
{
	// ���ӵ��� �����մϴ�. ��Ģ�� ���� ȣ�� ����ŭ ȣ���ؾ��ϱ� ������ ���� �׷��� ���� �ʾƵ� �ȴٰ� �մϴ�.
	::WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	// ��Ÿ�ӿ� ��� ������ ���� ä���� �Լ� 

	DWORD bytes = 0;
	// �Ѱ��ִ� ���ڵ��� �䵥 �ϳ��ϳ� ���� �Ѿ�� �����̴ϴ�. ���߿� �ʿ��Ҷ� ������ ã�ƺ��°ɷ� �ϰڽ��ϴ�.
	// ��� �� �Լ��� �ϴ� ���� ���� connectEx, disconnectEx, acceptEx �Լ��� ��Ÿ�ӿ� �ҷ����� ���� �Լ��Դϴ�.
	// SOCKET_ERROR �� �������� bool �� ��ȯ�ϰ� �ֽ��ϴ�.
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	/*
	���ݱ��� ��� ���� ����� �Լ��� ::socket() �̾��µ� 
	::WSASocket() �Լ��ε� ������ ����� �ְ� �� �Լ��� ����� ���� ����ȭ�� �ɼ��� ���� �� �ֽ��ϴ�. 
	���ڸ��
	1) AF				: �ּ� �йи� ��� ���� ���� IPv4, IPv6 ���� ����ִ� �ɼ�
	2) type				: �� ������ ���� ��� TCP, UDP �� ����ִ� �ɼ�
	3) protocol			:	����� �������� �������� 0�� �༭ �˾Ƽ� TCP�� ����ֵ��� �ߴµ� 
						�̹��� ���� �ɼ��� TCP�� �����߽��ϴ�.
	4) lpProtocolInfo	: ���� ������ Ư���� ���������� ���ϴ� WSAPROTOCOL_INFO ����ü�� 
						���� �����͸� �޽��ϴ�. NULL�� �༭ ���� �ɼ��� �ǵ帮�� �ʾҽ��ϴ�. 
	5) g				: �� ���� �� �� ���� �׷��� ������ �ƴ���, ����ٸ� ID �߱� ������ ��� ������ ����
	6) dwFlags			: �߰����� ������ Ư������ �����ϴµ� ���Ǵ� �÷��� �������� �̰͵� 0���� 
						���� �������� �ʾҾ����ϴ�. ���� ���ڷ� �־��� WSA_FLAG_OVERLAPPED �� 
						�񵿱� IO �Լ��� �����ϴ� ������ �ɼ��Դϴ�.

	��ȯ���� ������ ��ȯ�մϴ�.
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

// listenSocket �� ���� �ɼ��� ClientSocket�� �״�� �����Ѵٴ� �ǹ� 
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN)) ;
}

// bind �� �ʿ��� SOCKADDR_IN ����ü�� ������ �ڽ��� ������ �ִ� �ƹ� �ּҷ� �޾Ƽ� bind ���� ���ݴϴ�. 
bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<SOCKADDR*>(&myAddress), sizeof(myAddress));
}

// listen �� ���ָ鼭 �� ����� �������� �ƴ����� boolŸ������ ��ȯ�մϴ�. 
bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

// ���ڸ� ������ �޾ƿ��� �ߺ��ؼ� closesocket ���� �ʵ��� üũ, 
// ���� socket�� ���� INVALID_SOCKET���� �������ְ� �ֽ��ϴ�. 
void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}
