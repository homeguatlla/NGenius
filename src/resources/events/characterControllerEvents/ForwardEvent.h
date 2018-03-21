#pragma once
#include "../CharacterControllerEvent.h"

class ForwardEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ForwardEvent();
	~ForwardEvent();

	void SetPressed(bool pressed);
	bool IsPressed() const;

	ForwardEvent* DoClone(const void* data) const override;
};

