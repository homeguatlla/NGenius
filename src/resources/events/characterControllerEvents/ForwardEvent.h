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

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ForwardEvent"); }
	static std::shared_ptr<GameEvent> Create();
};

