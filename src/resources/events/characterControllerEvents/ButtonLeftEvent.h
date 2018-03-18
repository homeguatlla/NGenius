#pragma once
#include "../CharacterControllerEvent.h"

class ButtonLeftEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ButtonLeftEvent();
	~ButtonLeftEvent();

	ButtonLeftEvent* DoClone(const void* data) const override;
	
	void SetAction(int action);
	bool IsPressed() const;
	bool IsReleased() const;
};

