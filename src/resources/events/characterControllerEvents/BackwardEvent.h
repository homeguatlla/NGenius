#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class BackwardEvent : public CharacterControllerEvent
{
public:
	BackwardEvent() = default;
	~BackwardEvent() = default;

	static std::string GetClassName() { return std::string("BackwardEvent"); }
	static std::shared_ptr<GameEvent> Create();

	std::shared_ptr<GameEvent> DoClone(const void* data) const;
};

