#pragma once
#include "../CharacterControllerEvent.h"

class StopEvent : public CharacterControllerEvent
{
public:
	StopEvent();
	~StopEvent();

	StopEvent* DoClone() const override;
};

