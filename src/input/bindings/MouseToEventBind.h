#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class MouseToEventBind : public InputConverter
{
	int mButton;
	int mAction;
	GameEvent* mEvent;

public:
	MouseToEventBind(int button, int action, GameEvent* event);
	MouseToEventBind(int button, GameEvent* event);
	~MouseToEventBind();

	GameEvent* Convert(const void* data) const override;
};

