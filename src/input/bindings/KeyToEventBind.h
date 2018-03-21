#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class KeyToEventBind : public InputConverter
{
	int mKey;
	const GameEvent* mEvent;

public:
	KeyToEventBind(int key, const GameEvent* event);
	~KeyToEventBind();

	const GameEvent* Convert(int key, int action) const;
};

