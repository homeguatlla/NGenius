#pragma once
#include "../InputEvent.h"

class BackwardEvent : public InputEvent
{
public:
	BackwardEvent();
	~BackwardEvent();

	InputEvent* DoClone() const;
};

