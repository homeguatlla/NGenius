#pragma once
#include "../CharacterControllerEvent.h"

class ForwardEvent : public CharacterControllerEvent
{
public:
	ForwardEvent();
	~ForwardEvent();

	ForwardEvent* DoClone() const override;
};

