#pragma once
#include "../CharacterControllerEvent.h"

class JumpEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	JumpEvent();
	~JumpEvent();

	void SetPressed(bool pressed);
	bool IsPressed() const;

private:
	JumpEvent* DoClone(const void* data) const override;
};

