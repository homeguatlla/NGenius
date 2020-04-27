#pragma once
#include <memory>

class GameEvent;

class InputConverter
{
public:
	InputConverter();
	~InputConverter();

	virtual std::shared_ptr<GameEvent> Convert(int key, int action) const { return nullptr; }
	virtual std::shared_ptr<GameEvent> Convert(const void* data) const { return nullptr; }
};

