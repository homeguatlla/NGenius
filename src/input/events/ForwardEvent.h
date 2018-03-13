#pragma once
#include "../InputEvent.h"

class ForwardEvent : public InputEvent
{
public:
	ForwardEvent();
	~ForwardEvent();

	InputEvent* DoClone() const override;
};

