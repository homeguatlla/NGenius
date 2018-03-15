#pragma once
class GameEvent;

class InputConverter
{
public:
	InputConverter();
	~InputConverter();

	virtual const GameEvent* Convert(int key, int action) const = 0;
};

