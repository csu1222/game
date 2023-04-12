#pragma once

/*---------------------------
	RefCountable
----------------------------*/

class RefCountable
{
public:
	RefCountable() : _refCount(1) { }
	virtual ~RefCountable() { }
	// 메모리릭을 방지하기 위해 상위 클래스 소멸자에는 virtual을 붙였어야 했습니다. 

	int32 GetRefCount() { return _refCount; }

	int32 AddRefCount() { return _refCount++; }

	int32 ReleaseRefCount()
	{
		int32 refCount = --_refCount;
		if (refCount == 0)
		{
			delete this;
		}
		return refCount;
	}
protected:
	atomic<int32> _refCount;
};

/*-------------------
	Shared Pointer
---------------------*/

template<typename T>
class TSharedPtr
{
public:

	TSharedPtr() { }
	TSharedPtr(T* ptr) { Set(ptr); }

	// 복사
	TSharedPtr(const TSharedPtr& rhs) { Set(rhs._ptr); }
	// 이동
	TSharedPtr(TSharedPtr&& rhs) { _ptr = rhs._ptr; rhs._ptr = nullptr; }
	// 상속 관계 복사
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { Set(static_cast<T*>(rhs._ptr)); }

	~TSharedPtr() { Release(); }

public:
	// 복사 연산자 
	TSharedPtr& operator= (const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}
		return *this;
	}

	// 이동 연산자
	TSharedPtr& operator=(TSharedPtr&& rhs)
	{
		Release();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	// 비교 연산자 
	bool		operator==(const TSharedPtr& rhs) const { return _ptr == rhs._ptr; }
	bool		operator==(T* ptr) const { return _ptr == ptr; }
	bool		operator!=(const TSharedPtr& rhs) const { return _ptr != rhs._ptr; }
	bool		operator!=(T* ptr) const { return _ptr != ptr; }
	bool		operator<(const TSharedPtr& rhs) const { return _ptr < rhs._ptr; }
	bool		operator>(const TSharedPtr& rhs) const { return _ptr > rhs._ptr; }


	// 생 포인터 반환 연산자
	T* operator*() { return _ptr; }
	const T* operator*() const { return _ptr; }
	operator T* () const { return _ptr; }
	T* operator->() { return _ptr; }
	const T* operator->() const { return _ptr; }

	bool IsNull() { return _ptr == nullptr; }


private:
	inline void Set(T* ptr)
	{
		_ptr = ptr;
		if (ptr)
			ptr->AddRefCount();
	}

	inline void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRefCount();
			_ptr = nullptr;
		}
	}

private:
	T* _ptr = nullptr;
};