#pragma once
#include "../../resources/GameEvent.h"
#include "../InputConverter.h"

class KeyToEventBind : public InputConverter
{
	int mKey;
	std::shared_ptr<GameEvent> mEvent;

public:
	KeyToEventBind(int key, std::shared_ptr<GameEvent> event);
	~KeyToEventBind() = default;

	std::shared_ptr<GameEvent> Convert(int key, int action) const;
};

