#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class KeyToEventBind : public InputConverter
{
protected:
	int mKey;
	GameEvent* mEvent;

public:
	KeyToEventBind(int key, GameEvent* event);
	~KeyToEventBind();

	virtual GameEvent* Convert(int key, int action) const;
};

