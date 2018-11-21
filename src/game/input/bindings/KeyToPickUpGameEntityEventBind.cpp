#include "stdafx.h"
#include "KeyToPickUpGameEntityEventBind.h"
#include "../../events/PickupGameEntityEvent.h"
#include "../../../resources/GameEntity.h"
#include "../../../NGenius.h"
#include "../../ShooterGameConstants.h"
#include "../../Player.h"
#include <GLFW/glfw3.h>
#include <vector>

KeyToPickUpGameEntityEventBind::KeyToPickUpGameEntityEventBind(int key, GameEvent* event, Player* player) : 
	KeyToEventBind(key, event),
	mPlayer(player)
{
}


KeyToPickUpGameEntityEventBind::~KeyToPickUpGameEntityEventBind()
{
}

GameEvent* KeyToPickUpGameEntityEventBind::Convert(int key, int action) const
{
	if (mKey == key)
	{
		if (action == GLFW_PRESS)
		{
			std::vector<std::pair<GameEntity*, float>> entities;
			NGenius::GetInstance().FillWithGameEntitiesVisibleInsideRadius(
				mPlayer->GetEntity()->GetTransformation()->GetPosition(),
				PLAYER_PICKUP_RADIUS,
				entities,
				true);
			if (!entities.empty())
			{
				const void* data = reinterpret_cast<const void*>(action);
				PickupGameEntityEvent* event = static_cast<PickupGameEntityEvent*>(mEvent->Clone(data));
				event->SetGameEntity(entities[0].first);

				return event;
			}
		}
	}

	return nullptr;
}
