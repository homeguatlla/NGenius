#include "stdafx.h"
#include "DamageSystem.h"
#include "../../resources/GameEntity.h"
#include "../../resources/components/GameEventsComponent.h"
#include "../components/DamageComponent.h"
#include "../components/HealthComponent.h"
#include <algorithm>
#include <assert.h>

DamageSystem::DamageSystem() :
	mIsInitialized(false)
{
}

DamageSystem::~DamageSystem()
{
	mEntities.clear();
}

void DamageSystem::Start()
{
	mIsInitialized = true;
}

void DamageSystem::Update(float elapsedTime)
{
	assert(mIsInitialized);

	for (GameEntity* entity : mEntities)
	{
		DamageComponent* damageComponent = entity->GetComponent<DamageComponent>();
		if(damageComponent->GetDamage() > 0)
		{
		}
	}
}

bool DamageSystem::HasDamageComponents(const GameEntity* entity) const
{
	return entity != nullptr && 
		entity->HasComponent<DamageComponent>() && 
		entity->HasComponent<HealthComponent>() && 
		entity->HasComponent<GameEventsComponent>();
}

void DamageSystem::AddEntity(GameEntity* entity)
{
	mEntities.push_back(entity);
}

void DamageSystem::RemoveEntity(GameEntity* entity)
{
	if (HasDamageComponents(entity))
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

void DamageSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasDamageComponents(entity))
	{
		AddEntity(entity);
	}
}

void DamageSystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasDamageComponents(entity))
	{
		RemoveEntity(entity);
	}
}