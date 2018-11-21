#include "stdafx.h"
#include "PickupGameEntityEvent.h"


PickupGameEntityEvent::PickupGameEntityEvent() : mGameEntity(nullptr)
{
}


PickupGameEntityEvent::~PickupGameEntityEvent()
{
}

GameEntity* PickupGameEntityEvent::GetGameEntity() const
{
	return mGameEntity;
}

void PickupGameEntityEvent::SetGameEntity(GameEntity* entity)
{
	mGameEntity = entity;
}

PickupGameEntityEvent* PickupGameEntityEvent::DoClone(const void* data) const
{
	PickupGameEntityEvent* event = new PickupGameEntityEvent();
	event->SetGameEntity(GetGameEntity());

	return event;
}