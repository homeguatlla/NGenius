#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class KeyToEventBind : public InputConverter
{
	int mKey;
	std::shared_ptr<const GameEvent> mEvent;

public:
	KeyToEventBind(int key, std::shared_ptr<const GameEvent> event);
	~KeyToEventBind() = default;

	std::shared_ptr<const GameEvent> Convert(int key, int action) const;
};

