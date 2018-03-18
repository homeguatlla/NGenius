#pragma once
#include "../CharacterControllerEvent.h"

class ButtonRightEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ButtonRightEvent();
	~ButtonRightEvent();

	ButtonRightEvent* DoClone(const void* data) const override;
	
	void SetAction(int action);
	bool IsPressed() const;
	bool IsReleased() const;
};

