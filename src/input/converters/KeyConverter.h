#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class KeyConverter : public InputConverter
{
	int mKey;
	int mAction;
	const GameEvent* mEvent;

public:
	KeyConverter(int key, int action, const GameEvent* event);
	~KeyConverter();

	const GameEvent* Convert(int key, int action) const;
};

