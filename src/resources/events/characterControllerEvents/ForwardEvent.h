#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ForwardEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ForwardEvent();
	~ForwardEvent();

	void SetPressed(bool pressed);
	bool IsPressed() const;

	ForwardEvent* DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ForwardEvent"); }
	static GameEvent* Create();
};

