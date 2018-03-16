#pragma once
#include "../CharacterControllerEvent.h"

class ZoomEvent : public CharacterControllerEvent
{
public:
	ZoomEvent();
	~ZoomEvent();

	ZoomEvent* DoClone() const override;
};

