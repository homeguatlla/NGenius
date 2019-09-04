#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ButtonLeftEvent : public CharacterControllerEvent
{
	bool mIsPressed;

public:
	ButtonLeftEvent();
	~ButtonLeftEvent();

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ButtonLeftEvent"); }
	static std::shared_ptr<GameEvent> Create();
	
	void SetAction(int action);
	bool IsPressed() const;
	bool IsReleased() const;
};

