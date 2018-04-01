#pragma once
#include "BaseVisitor.h"

template<typename R = void>
class BaseVisitable
{
public:
	typedef R ReturnType;

	BaseVisitable() {};
	virtual ~BaseVisitable() {};
	virtual R Accept(BaseVisitor&) = 0;

protected:
	template<class T>
	static ReturnType AcceptImpl(T& visited, BaseVisitor& guest)
	{
		//Apply the Acyclic visitor
		Visitor<T, R>* p = dynamic_cast<Visitor<T, R>*>(&guest);
		if (p != nullptr)
		{
			return p->Visit(visited);
		}
		return ReturnType();
	}
};

