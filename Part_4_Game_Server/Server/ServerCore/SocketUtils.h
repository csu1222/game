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
	// ���� �񵿱� �Լ��� �����ͷ� ������ ����
	// ó������ Null �������� ��Ÿ�ӿ� �ּҸ� �ܾ�;���
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;
public:
	// WinSock �ʱ�ȭ�� ������ ���� �Լ��� ������ CoreGlobal �ʿ��� ���ٰ��Դϴ�.
	static void Init();
	static void Clear();

	// ��Ÿ�ӿ� ���� ������ ������ ���� ä���� �Լ�
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
	// ������ �����ϴ� �Լ� WSASocket ���
	static SOCKET CreateSocket();

	// ���� �ɼ��� �����ϴ� �Լ��� 
	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	// Bind, Listen,Close ���� �Լ��� �����ؼ� ����մϴ�. 
	static bool Bind(SOCKET socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

// ������ ������ �ɼ��� �̸� ���� �����ϴ� �Լ��� ���ø��� ����ϵ��� �����մϴ�. 
template<typename T>
static inline bool SetSocketOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}