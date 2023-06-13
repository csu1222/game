#pragma once


/*
------------------
	Net Address
------------------
*/
// 역할
// 주소를 관리하던 SOCKADDR_IN 구조체
// 여러 클라이언트가 접속했을때 클라이언트의 주소를 일일히 구조체로 만들어주기 보다
// 이 클래스를 통해 편하게 설정을 하기 위함
class NetAddress
{
public:
	// 여러종류의 생성자
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(wstring ip, uint16 port);

	// Get 함수들 
	SOCKADDR_IN&	GetSockAddr() { return _sockAddr; }
	wstring			GetIpAddress();
	uint16			GetPort() { return ::ntohs(_sockAddr.sin_port); }

public:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN		_sockAddr = {};
};

