#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"


using WraightRef = TSharedPtr<Wraight>;
using MissileRef = TSharedPtr<Missile>;

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
};

class Missile : public RefCountable
{
public:
	void SetTarget(WraightRef target)
	{
		_target = target;
		//target->AddRefCount();
	}
	
	bool Update()
	{
		if (_target == nullptr)
			return true;

		int posX = _target->_posX;
		int posY = _target->_posY;

		// TODO : 쫓아가기

		if (_target->_hp == 0)
		{
			_target->ReleaseRefCount();
			_target = nullptr;
			return true;
		}

		return false;
	}

	WraightRef _target = nullptr;

};

int main()
{
	WraightRef wraight(new Wraight());
	wraight->ReleaseRefCount();
	MissileRef missile(new Missile());
	missile->ReleaseRefCount();

	missile->SetTarget(wraight);

	// 레이스가 피격 당함
	wraight->_hp = 0;
	//delete wraight;
	//wraight->ReleaseRefCount();
	wraight = nullptr;

	while (true)
	{
		if (missile)
		{
			if (missile->Update())
			{
				//missile->ReleaseRefCount();
				missile = nullptr;

				break;
			}
		}
	}

	//delete missile;
}