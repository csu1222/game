#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
// ServerService �� ����ϱ� ����
class ServerService;

/*
--------------------
	Listener �����
--------------------
*/
// listenSocket ���� ������ �ϰ� �ɰ̴ϴ�. 
// �� Ŭ������ CP�� ��ϵǾ� �۾��� �Ұ��̱⿡ IocpObject�� ��ӹ޾ҽ��ϴ�.
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* �ܺο��� ��� */
	// NetAddress ��� ServerServiceRef �� ���ڷ� �޽��ϴ�. 
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();


public:
	// IocpObject�� ����߱� ������ �ݵ�� ����������ϴ� �Լ��� 
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

	
protected:
	// ��� ������ 
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
	// TODO : ���߿� ���� ī��Ʈ ���� �����͵� ��� ���� �� �ֽ��ϴ�. 

	// StartAccept ���� ���� ServerService�� ������ ��� �����̴ϴ�.
	// ������ ServerService ������ Listener�� ��� �ְ� ���⼭�� ServerService�� ��� �ְԵǸ� 
	// shared_ptr �δ� ���� ��ȯ ������ ���� �� �ֽ��ϴ�. �̰� �����ؼ� �����ϰڽ��ϴ�. 
	ServerServiceRef _service;

};

