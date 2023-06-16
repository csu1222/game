#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;

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
	// Listener���� ������ ������ �ض� ��� �Լ�
	bool StartAccept(NetAddress netAddress);
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

};

