#pragma once
#include "Session.h"

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;
// GameSession ���� ���� �����ϴ� �Ŵ����� �ɰ��Դϴ�. 
// ���� �̹� Service��� Ŭ������ Session �Ӹ��ƴ϶� �ּ� ����ü, ����, iocpCore ���� �����ϰ� �־�����
// GameSessionManager�� �� �� �������ʿ��� ���ϰ� ����ϱ� ���� ���� �����մϴ�. 
class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
};

// ����ϱ� ���� ���� ��ü�� ����� ����ϴ�. 
extern GameSessionManager GSessionManager;