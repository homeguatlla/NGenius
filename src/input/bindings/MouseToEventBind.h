#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class MouseToEventBind : public InputConverter
{
	int mButton;
	int mAction;
	const GameEvent* mEvent;

public:
	MouseToEventBind(int button, int action, const GameEvent* event);
	MouseToEventBind(int button, const GameEvent* event);
	~MouseToEventBind();

	const GameEvent* Convert(const void* data) const override;
};

