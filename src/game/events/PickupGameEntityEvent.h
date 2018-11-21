#pragma once
#include "../../resources/events/CharacterControllerEvent.h"

class GameEntity;

class PickupGameEntityEvent : public CharacterControllerEvent
{
	GameEntity* mGameEntity;

public:
	PickupGameEntityEvent();
	virtual ~PickupGameEntityEvent();

	PickupGameEntityEvent* DoClone(const void* data) const override;

	void SetGameEntity(GameEntity* entity);
	GameEntity* GetGameEntity() const;
};

