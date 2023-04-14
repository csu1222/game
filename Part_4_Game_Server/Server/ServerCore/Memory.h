#pragma once
#include "Allocator.h"
#include "CoreMacro.h"

template<typename Type, typename... Args>
Type* A_new(Args&&... args)
{
	Type* memory =	static_cast<Type*>(A_alloc(sizeof(Type)));
	
	// placement new	할당된 메모리에 생성자를 호출해주는 문법 
	// new(객체를 만들어줄 공간) 객체의 템플릿(std::forward<Args>(인자들)...);
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

template<typename Type>
void A_delete(Type* obj)
{
	obj->~Type();
	A_release(obj);
}