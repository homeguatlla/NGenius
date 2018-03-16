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
	mNewEntitiesToAdd.clear();
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

void InputSystem::OnKey(int key, int action)
{
	for (GameEntity* entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		CharacterComponent* characterComponent = entity->GetComponent<CharacterComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			const GameEvent* event = inputComponent->ConvertKey(key, action);
			if (event != nullptr)
			{
				characterComponent->OnCharacterControllerEvent(event);
			}
		}
	}
}

void InputSystem::OnMouseScroll(int button, float scroll)
{
	for (GameEntity* entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		CharacterComponent* characterComponent = entity->GetComponent<CharacterComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			const GameEvent* event = inputComponent->ConvertMouse(button, scroll);
			if (event != nullptr)
			{
				characterComponent->OnCharacterControllerEvent(event);
			}
		}
	}
}