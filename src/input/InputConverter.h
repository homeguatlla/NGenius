#pragma once
class GameEvent;

class InputConverter
{
public:
	InputConverter();
	~InputConverter();

	virtual const GameEvent* Convert(int key, int action) const { return nullptr; }
	virtual const GameEvent* Convert(const void* data) const { return nullptr; }
};

