#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class RightEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	RightEvent();
	virtual ~RightEvent() = default;

	void SetPressed(bool pressed);
	bool IsPressed() const;

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("RightEvent"); }
	static std::shared_ptr<GameEvent> Create();
};

