#pragma once
class InputEvent
{
public:
	InputEvent();
	~InputEvent();

	template<typename T>
	bool IsOfType() const
	{
		return &typeid(T) == &typeid(this);
	}
};

