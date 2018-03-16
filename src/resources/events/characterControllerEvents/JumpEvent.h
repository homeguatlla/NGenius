#pragma once
#include "../CharacterControllerEvent.h"

class JumpEvent : public CharacterControllerEvent
{
public:
	JumpEvent();
	~JumpEvent();

	JumpEvent* DoClone() const override;
};

