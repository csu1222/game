#include "pch.h"
#include "NetAddress.h"


/*
------------------
	Net Address
------------------
*/

// SOCKADDR_IN을 인자로 받은경우는 그냥 바로 넣어줍니다. 
NetAddress::NetAddress(SOCKADDR_IN sockAddr) : _sockAddr(sockAddr)
{
}

// ip와 port 만 받은경우는 직접 _sockAddr을 채워줍니다. 
NetAddress::NetAddress(wstring ip, uint16 port)
{
	::memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr = Ip2Address(ip.c_str());
	_sockAddr.sin_port = ::htons(port);
}

wstring NetAddress::GetIpAddress()
{
	WCHAR buffer[100];
	/*
	네번째 인자로 버퍼의 사이즈를 넣어주는데 버퍼가 WCHAR 타입(2byte) 배열입니다. 
	즉 200 바이트라는 사이즈인데 의도했던 사이즈는 100 입니다. 
	제대로 넣어주기 위해서는 sizeof(buffer) / sizeof(WCHAR) 를 넣어줘야 하는데 
	일일히 타이핑하기에는 위험하다 보니 매크로를 만들었습니다. 
	*/
	::InetNtopW(AF_INET, &_sockAddr, buffer, len32(buffer));
	return wstring(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}
