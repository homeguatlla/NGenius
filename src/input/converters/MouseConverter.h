#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class MouseConverter : public InputConverter
{
	int mButton;
	int mAction;
	float mScroll;
	const GameEvent* mEvent;

public:
	MouseConverter(int button, int action, const GameEvent* event);
	MouseConverter(int button, const GameEvent* event);
	~MouseConverter();

	const GameEvent* Convert(int button, int action) const;
	const GameEvent* MouseConverter::Convert(int button, float scroll) const;
};

