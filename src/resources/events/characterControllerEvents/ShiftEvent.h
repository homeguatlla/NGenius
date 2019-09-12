#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ShiftEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ShiftEvent();
	virtual ~ShiftEvent() = default;

	void SetPressed(bool pressed);
	bool IsPressed() const;

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ShiftEvent"); }
	static std::shared_ptr<GameEvent> Create();
};

