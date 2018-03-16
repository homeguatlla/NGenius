#pragma once
class IInputListener
{
public:
	IInputListener();
	~IInputListener();

	virtual void OnKey(int key, int action) = 0;
	virtual void OnMouseScroll(int button, float scroll) = 0;
};

