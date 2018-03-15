#pragma once
#include "../../resources/GameEvent.h"

class BackwardEvent : public GameEvent
{
public:
	BackwardEvent();
	~BackwardEvent();

	GameEvent* DoClone() const;
};

