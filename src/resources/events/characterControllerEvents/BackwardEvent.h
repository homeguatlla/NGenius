#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class BackwardEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	BackwardEvent();
	~BackwardEvent();

	void SetPressed(bool pressed);
	bool IsPressed() const;
	static std::string GetClassName() { return std::string("BackwardEvent"); }
	static GameEvent* Create();

	BackwardEvent* DoClone(const void* data) const;
};

