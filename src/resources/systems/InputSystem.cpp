#include "stdafx.h"
#include "InputSystem.h"
#include "../GameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/InputComponent.h"
#include "../components/CharacterComponent.h"

#include <algorithm>

InputSystem::InputSystem(InputHandler* inputHandler)
{
	inputHandler->RegisterAllEventsInputListener(this);
}

InputSystem::~InputSystem()
{
	ReleaseEntities(&mEntities);
	ReleaseEntities(&mNewEntitiesToAdd);
	mEntitiesToRemove.clear();
	mEntities.clear();
}

void InputSystem::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();
}

bool InputSystem::HasInputComponents(GameEntity* entity) const
{
	return entity != nullptr && entity->HasComponent<InputComponent>() && entity->HasComponent<CharacterComponent>();
}

void InputSystem::AddEntity(GameEntity* entity)
{
	mNewEntitiesToAdd.push_back(entity);
}

void InputSystem::RemoveEntity(GameEntity* entity)
{
	mEntitiesToRemove.push_back(entity);
}

void InputSystem::AddNewEntities()
{
	for (GameEntity* entity : mNewEntitiesToAdd)
	{
		mEntities.push_back(entity);
	}
	mNewEntitiesToAdd.clear();
}

void InputSystem::RemoveEntities()
{
	for (GameEntity* entity : mEntitiesToRemove)
	{
		GameEntitiesIterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		mEntities.erase(it);
	}
	mEntitiesToRemove.clear();
}

void InputSystem::ReleaseEntities(std::vector<GameEntity*>* entities)
{
	for (GameEntity* entity : *entities)
	{
		delete entity;
	}
	entities->clear();

}
void InputSystem::OnKey(int key, int action)
{
	for (GameEntity* entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		CharacterComponent* characterComponent = entity->GetComponent<CharacterComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			const GameEvent* event = inputComponent->Convert(key, action);
			characterComponent->OnGameEvent(event);
		}
	}
}

void InputSystem::OnMouseScroll(float scroll)
{

}