#include "stdafx.h"
#include "EntitiesSystem.h"
#include "renderSystem/RenderSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "../GameEntity.h"
#include "../components/LODComponent.h"

#include <algorithm>

EntitiesSystem::EntitiesSystem(RenderSystem* renderSystem, PhysicsSystem* physicsSystem, InputSystem* inputSystem) :
mRenderSystem(renderSystem),
mPhysicsSystem(physicsSystem),
mInputSystem(inputSystem)
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
		if (mInputSystem->HasInputComponents(entity))
		{
			mInputSystem->AddEntity(entity);
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
		mInputSystem->RemoveEntity(*it);
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
