#pragma once
#include "../CharacterControllerEvent.h"

class BackwardEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	BackwardEvent();
	~BackwardEvent();

	void SetPressed(bool pressed);
	bool IsPressed() const;

	BackwardEvent* DoClone(const void* data) const;
};

