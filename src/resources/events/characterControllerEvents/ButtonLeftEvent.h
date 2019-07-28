#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ButtonLeftEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ButtonLeftEvent();
	~ButtonLeftEvent();

	ButtonLeftEvent* DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ButtonLeftEvent"); }
	static GameEvent* Create();
	
	void SetAction(int action);
	bool IsPressed() const;
	bool IsReleased() const;
};

