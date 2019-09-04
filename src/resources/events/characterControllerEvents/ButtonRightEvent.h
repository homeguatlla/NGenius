#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ButtonRightEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ButtonRightEvent();
	~ButtonRightEvent();

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ButtonRightEvent"); }
	static std::shared_ptr<GameEvent> Create();
	
	void SetAction(int action);
	bool IsPressed() const;
	bool IsReleased() const;
};

