#pragma once
class BaseVisitor
{
public:
	BaseVisitor();
	virtual ~BaseVisitor();
};

template <class T, typename R = void>
class Visitor
{
public:
	typedef R ReturnType;
	virtual ReturnType Visit(const T&) = 0;
};

