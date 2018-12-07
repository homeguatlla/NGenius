#include "stdafx.h"
#include "ISystem.h"

#include <assert.h>
#include <algorithm>

ISystem::ISystem()
{
}


ISystem::~ISystem()
{
}

void ISystem::AddEntity(GameEntity* entity)
{
	if (HasSpecificComponents(entity))
	{
		mEntities.push_back(entity);
	}
}

void ISystem::RemoveEntity(GameEntity* entity)
{
	if (HasSpecificComponents(entity))
	{
		std::vector<GameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		if (it != mEntities.end())
		{
			mEntities.erase(it);
		}
		else
		{
			assert(false);
		}
	}
}

void ISystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasSpecificComponents(entity))
	{
		AddEntity(entity);
	}
}

void ISystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasSpecificComponents(entity))
	{
		RemoveEntity(entity);
	}
}