#pragma once
#include "src/resources/GameEvent.h"

class CharacterControllerEvent : public GameEvent
{
public:
	CharacterControllerEvent();
	~CharacterControllerEvent() = default;

	void SetPressed(bool pressed);
	bool IsPressed() const;

private:
	bool mIsPressed;
};

