#pragma once
#include "../../resources/GameEvent.h"

class ForwardEvent : public GameEvent
{
public:
	ForwardEvent();
	~ForwardEvent();

	GameEvent* DoClone() const override;
};

