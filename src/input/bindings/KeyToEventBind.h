#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class KeyToEventBind : public InputConverter
{
	int mKey;
	int mAction;
	const GameEvent* mEvent;

public:
	KeyToEventBind(int key, int action, const GameEvent* event);
	~KeyToEventBind();

	const GameEvent* Convert(int key, int action) const;
};

