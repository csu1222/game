#pragma once
#include <functional>

/*----------
	Job
-----------*/

// 람다식에서는 따로 인자를 받기보다는 캡쳐를 해 사용하기때문에 
// 반환 void, 매개변수 void 형으로 만들었습니다.
using CallbackType = std::function<void()>;

class Job
{
public:
	Job(CallbackType&& callback) : _callback(std::move(callback))
	{
	}

	// shared_ptr을 관리해주는 버전
	template<typename T, typename Ret, typename... Args>
	Job(shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
	{
		_callback = [owner, memFunc, args...]()
		{
			(owner.get()->*memFunc)(args...);
		};
	}

	void Execute()
	{
		_callback();
	}

private:
	CallbackType _callback;
};

