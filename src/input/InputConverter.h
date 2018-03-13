#pragma once
class InputEvent;

class InputConverter
{
public:
	InputConverter();
	~InputConverter();

	virtual const InputEvent* Convert(int key, int action) const = 0;
};

