#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class MouseConverter : public InputConverter
{
	int mButton;
	int mAction;
	const GameEvent* mEvent;

public:
	MouseConverter(int button, int action, const GameEvent* event);
	MouseConverter(int button, const GameEvent* event);
	~MouseConverter();

	const GameEvent* Convert(const void* data) const override;
};

