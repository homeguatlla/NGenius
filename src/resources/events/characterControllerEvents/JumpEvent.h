#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class JumpEvent : public CharacterControllerEvent
{
public:
	JumpEvent();
	~JumpEvent();

	JumpEvent* DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("JumpEvent"); }
	static GameEvent* Create();
};

