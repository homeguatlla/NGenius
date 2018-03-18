#pragma once
class IInputListener
{
public:
	IInputListener();
	~IInputListener();

	virtual void OnKey(int key, int action) = 0;
	virtual void OnMouseScroll(int button, float scroll) = 0;
	virtual void OnMouseButton(int button, int action, int mods) = 0;
	virtual void OnMouseCursorPos(double x, double y) = 0;
};

