#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class LeftEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	LeftEvent();
	virtual ~LeftEvent() = default;

	void SetPressed(bool pressed);
	bool IsPressed() const;

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("LeftEvent"); }
	static std::shared_ptr<GameEvent> Create();
};

