#include "pch.h"
#include "NetAddress.h"


/*
------------------
	Net Address
------------------
*/

// SOCKADDR_IN�� ���ڷ� �������� �׳� �ٷ� �־��ݴϴ�. 
NetAddress::NetAddress(SOCKADDR_IN sockAddr) : _sockAddr(sockAddr)
{
}

// ip�� port �� �������� ���� _sockAddr�� ä���ݴϴ�. 
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
	�׹�° ���ڷ� ������ ����� �־��ִµ� ���۰� WCHAR Ÿ��(2byte) �迭�Դϴ�. 
	�� 200 ����Ʈ��� �������ε� �ǵ��ߴ� ������� 100 �Դϴ�. 
	����� �־��ֱ� ���ؼ��� sizeof(buffer) / sizeof(WCHAR) �� �־���� �ϴµ� 
	������ Ÿ�����ϱ⿡�� �����ϴ� ���� ��ũ�θ� ��������ϴ�. 
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
