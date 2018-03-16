#pragma once
#include "../CharacterControllerEvent.h"

class BackwardEvent : public CharacterControllerEvent
{
public:
	BackwardEvent();
	~BackwardEvent();

	BackwardEvent* DoClone() const;
};

