#pragma once
class GameEvent;

class InputConverter
{
public:
	InputConverter();
	~InputConverter();

	virtual GameEvent* Convert(int key, int action) const { return nullptr; }
	virtual GameEvent* Convert(const void* data) const { return nullptr; }
};

