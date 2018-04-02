#include "stdafx.h"
#include "EntitiesSystem.h"
#include "renderSystem/RenderSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "DebugSystem.h"
#include "../GameEntity.h"
#include "../components/LODComponent.h"

#include <algorithm>

EntitiesSystem::EntitiesSystem(RenderSystem* renderSystem, PhysicsSystem* physicsSystem, InputSystem* inputSystem, DebugSystem* debugSystem) :
mRenderSystem(renderSystem),
mPhysicsSystem(physicsSystem),
mInputSystem(inputSystem),
mDebugSystem(debugSystem)
{
}

EntitiesSystem::~EntitiesSystem()
{
	mEntitiesToRemove.clear(); //these entities were removed when releasing mEntities.
	ReleaseEntities(&mEntities);
	ReleaseEntities(&mNewEntitiesToAdd);	
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

unsigned int EntitiesSystem::GetNumberGameEntities() const
{
	return mEntities.size();
}

void EntitiesSystem::AddEntity(GameEntity* entity)
{
	entity->Init();
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
		if (mDebugSystem->HasDebugComponents(entity))
		{
			mDebugSystem->AddEntity(entity);
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
		mDebugSystem->RemoveEntity(*it);
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

BaseVisitable<>::ReturnType EntitiesSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}