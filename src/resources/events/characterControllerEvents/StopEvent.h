#pragma once
#include "../CharacterControllerEvent.h"

class StopEvent : public CharacterControllerEvent
{
public:
	StopEvent();
	~StopEvent();

	StopEvent* DoClone(const void* data) const override;
};

