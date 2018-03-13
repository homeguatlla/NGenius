#pragma once
#include "../InputEvent.h"

class ZoomEvent : public InputEvent
{
public:
	ZoomEvent();
	~ZoomEvent();

	InputEvent* DoClone() const;
};

