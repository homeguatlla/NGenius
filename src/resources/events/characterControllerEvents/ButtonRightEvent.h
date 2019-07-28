#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ButtonRightEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ButtonRightEvent();
	~ButtonRightEvent();

	ButtonRightEvent* DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ButtonRightEvent"); }
	static GameEvent* Create();
	
	void SetAction(int action);
	bool IsPressed() const;
	bool IsReleased() const;
};

