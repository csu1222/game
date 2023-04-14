#pragma once
#include "Allocator.h"
#include "CoreMacro.h"

template<typename Type, typename... Args>
Type* A_new(Args&&... args)
{
	Type* memory =	static_cast<Type*>(A_alloc(sizeof(Type)));
	
	// placement new	�Ҵ�� �޸𸮿� �����ڸ� ȣ�����ִ� ���� 
	// new(��ü�� ������� ����) ��ü�� ���ø�(std::forward<Args>(���ڵ�)...);
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

template<typename Type>
void A_delete(Type* obj)
{
	obj->~Type();
	A_release(obj);
}