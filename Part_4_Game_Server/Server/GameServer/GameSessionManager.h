#pragma once
#include "Session.h"

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;
// GameSession 들을 묶어 관리하는 매니저가 될것입니다. 
// 물론 이미 Service라는 클래스로 Session 뿐만아니라 주소 구조체, 소켓, iocpCore 등을 관리하고 있었지만
// GameSessionManager는 좀 더 컨텐츠쪽에서 편하게 사용하기 위해 묶어 관리합니다. 
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

// 사용하기 쉽게 전역 객체로 만들어 줬습니다. 
extern GameSessionManager GSessionManager;