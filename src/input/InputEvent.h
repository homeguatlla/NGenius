#pragma once
class InputEvent
{
public:
	InputEvent();
	~InputEvent();

	InputEvent* Clone() const;

	template<typename T>
	bool IsOfType() const
	{
		return typeid(T) == typeid(*this);
	}

private:
	virtual InputEvent* DoClone() const = 0;
};

