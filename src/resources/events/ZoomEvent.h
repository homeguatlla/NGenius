#pragma once
#include "../GameEvent.h"

class ZoomEvent : public GameEvent
{
public:
	ZoomEvent();
	~ZoomEvent();

	GameEvent* DoClone() const;
};

