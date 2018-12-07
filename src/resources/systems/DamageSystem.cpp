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

bool DamageSystem::HasToBeRegisteredToGameScene() const
{
	return true;
}

bool DamageSystem::HasSpecificComponents(const GameEntity* entity) const
{
	return entity != nullptr && 
		entity->HasComponent<DamageComponent>() && 
		entity->HasComponent<HealthComponent>() && 
		entity->HasComponent<GameEventsComponent>();
}

BaseVisitable<>::ReturnType DamageSystem::Accept(BaseVisitor & guest)
{
	return BaseVisitable<>::ReturnType();
}
