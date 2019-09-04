#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class JumpEvent : public CharacterControllerEvent
{
public:
	JumpEvent();
	~JumpEvent();

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("JumpEvent"); }
	static std::shared_ptr<GameEvent> Create();
};

