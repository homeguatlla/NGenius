#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class MouseToEventBind : public InputConverter
{
	int mButton;
	int mAction;
	std::shared_ptr<const GameEvent> mEvent;

public:
	MouseToEventBind(int button, int action, std::shared_ptr<const GameEvent> event);
	MouseToEventBind(int button, std::shared_ptr<const GameEvent> event);
	~MouseToEventBind() = default;

	std::shared_ptr<const GameEvent> Convert(const void* data) const override;
};

