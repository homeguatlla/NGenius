#include "stdafx.h"
#include "AttachGameEntityComponent.h"

#include <assert.h>

AttachGameEntityComponent::AttachGameEntityComponent(GameEntity* entity) : 
	mGameEntityAttached(entity)
{
	assert(entity != nullptr);
}

AttachGameEntityComponent::~AttachGameEntityComponent()
{
}

GameEntity* AttachGameEntityComponent::GetGameEntityAttached() const
{
	return mGameEntityAttached;
}

AttachGameEntityComponent* AttachGameEntityComponent::DoClone() const
{
	return new AttachGameEntityComponent(*this);
}
