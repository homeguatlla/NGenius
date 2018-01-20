#include "stdafx.h"
#include "EntitiesSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "../GameEntity.h"
#include "../components/LODComponent.h"

#include <algorithm>

EntitiesSystem::EntitiesSystem(RenderSystem* renderSystem, PhysicsSystem* physicsSystem) :
mRenderSystem(renderSystem),
mPhysicsSystem(physicsSystem)
{
}


EntitiesSystem::~EntitiesSystem()
{
	ReleaseEntities(&mEntities);
	ReleaseEntities(&mNewEntitiesToAdd);	
	mEntitiesToRemove.clear(); //these entities were removed when releasing mEntities.
}

void EntitiesSystem::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();

	for (GameEntity* entity : mEntities)
	{
		entity->Update(elapsedTime);
		IRenderer* renderer = entity->GetRenderer();
		mRenderSystem->AddToRender(renderer);
	}
}

void EntitiesSystem::AddEntity(GameEntity* entity)
{
	mNewEntitiesToAdd.push_back(entity);
}

void EntitiesSystem::RemoveEntity(GameEntity* entity)
{
	mEntitiesToRemove.push_back(entity);
}

void EntitiesSystem::AddNewEntities()
{
	for (GameEntity* entity : mNewEntitiesToAdd)
	{
		if (mPhysicsSystem->HasPhysicsComponents(entity))
		{
			mPhysicsSystem->AddEntity(entity);
		}
		mEntities.push_back(entity);
	}
	mNewEntitiesToAdd.clear();
}

void EntitiesSystem::RemoveEntities()
{
	for (GameEntity* entity : mEntitiesToRemove)
	{
		GameEntitiesIterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		mPhysicsSystem->RemoveEntity(*it);
		mEntities.erase(it);
	}
	mEntitiesToRemove.clear();
}

void EntitiesSystem::ReleaseEntities(std::vector<GameEntity*>* entities)
{
	for (GameEntity* entity : *entities)
	{
		delete entity;
	}
	entities->clear();
}
