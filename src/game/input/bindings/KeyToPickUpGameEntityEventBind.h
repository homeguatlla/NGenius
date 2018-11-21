#pragma once

#include "../../../input/bindings/KeyToEventBind.h"

class Player;

class KeyToPickUpGameEntityEventBind : public KeyToEventBind
{
	Player* mPlayer;

public:
	KeyToPickUpGameEntityEventBind(int key, GameEvent* event, Player* player);
	~KeyToPickUpGameEntityEventBind();

	GameEvent* Convert(int key, int action) const override;
};

