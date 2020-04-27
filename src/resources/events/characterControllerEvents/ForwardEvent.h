#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ForwardEvent : public CharacterControllerEvent
{
public:
	ForwardEvent() = default;
	virtual ~ForwardEvent() = default;

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ForwardEvent"); }
	static std::shared_ptr<GameEvent> Create();
};

