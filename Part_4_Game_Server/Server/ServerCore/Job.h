#pragma once
#include <functional>

/*----------
	Job
-----------*/

// ���ٽĿ����� ���� ���ڸ� �ޱ⺸�ٴ� ĸ�ĸ� �� ����ϱ⶧���� 
// ��ȯ void, �Ű����� void ������ ��������ϴ�.
using CallbackType = std::function<void()>;

class Job
{
public:
	Job(CallbackType&& callback) : _callback(std::move(callback))
	{
	}

	// shared_ptr�� �������ִ� ����
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

