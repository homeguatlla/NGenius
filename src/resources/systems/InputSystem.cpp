#include "stdafx.h"
#include "InputSystem.h"
#include "../GameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/InputComponent.h"
#include "../components/GameEventsComponent.h"

#include <algorithm>

InputSystem::InputSystem(InputHandler* inputHandler) :
	mInputHandler(inputHandler)
{
	mInputHandler->RegisterAllEventsInputListener(this);
}

InputSystem::~InputSystem()
{
	mEntities.clear();
	mInputHandler->UnRegisterInputListener(this);
}

void InputSystem::Update(float elapsedTime)
{
}

bool InputSystem::HasInputComponents(const GameEntity* entity) const
{
	return entity != nullptr && entity->HasComponent<InputComponent>() && entity->HasComponent<GameEventsComponent>();
}

void InputSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasInputComponents(entity))
	{
		AddEntity(entity);
	}
}

void InputSystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasInputComponents(entity))
	{
		RemoveEntity(entity);
	}
}

void InputSystem::AddEntity(GameEntity* entity)
{
	mEntities.push_back(entity);
}

void InputSystem::RemoveEntity(GameEntity* entity)
{
	if (HasInputComponents(entity))
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

void InputSystem::OnKey(int key, int action)
{
	for (GameEntity* entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		GameEventsComponent* characterComponent = entity->GetComponent<GameEventsComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			GameEvent* event = inputComponent->ConvertKey(key, action);
			if (event != nullptr)
			{
				characterComponent->OnCharacterControllerEvent(event);
			}
		}
	}
}

void InputSystem::OnMouseScroll(int button, float scroll)
{
	MouseData data(button, scroll);

	DispatchEvent(data);
}

void InputSystem::OnMouseButton(int button, int action, int mods)
{
	MouseData data(button, action, mods);

	DispatchEvent(data);
}

void InputSystem::OnMouseCursorPos(double x, double y)
{
	MouseData data(x, y);

	DispatchEvent(data);
}

void InputSystem::DispatchEvent(MouseData& data)
{
	for (GameEntity* entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		GameEventsComponent* characterComponent = entity->GetComponent<GameEventsComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			GameEvent* event = inputComponent->ConvertMouse(reinterpret_cast<void*>(&data));
			if (event != nullptr)
			{
				characterComponent->OnCharacterControllerEvent(event);
			}
		}
	}
}