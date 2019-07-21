#include "stdafx.h"
#include "InputSystem.h"
#include "../IGameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/InputComponent.h"
#include "../components/CharacterComponent.h"

#include <algorithm>
#include <assert.h>

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

bool InputSystem::HasInputComponents(const IGameEntity* entity) const
{
	return entity != nullptr && entity->HasComponent<InputComponent>() && entity->HasComponent<CharacterComponent>();
}

void InputSystem::OnGameEntityAdded(IGameEntity* entity)
{
	if (HasInputComponents(entity))
	{
		AddEntity(entity);
	}
}

void InputSystem::OnGameEntityRemoved(IGameEntity* entity)
{
	if (HasInputComponents(entity))
	{
		RemoveEntity(entity);
	}
}

void InputSystem::AddEntity(IGameEntity* entity)
{
	mEntities.push_back(entity);
}

void InputSystem::RemoveEntity(IGameEntity* entity)
{
	if (HasInputComponents(entity))
	{
		std::vector<IGameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](IGameEntity* a) { return a == entity; });
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
	for (IGameEntity* entity : mEntities)
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
	for (IGameEntity* entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		CharacterComponent* characterComponent = entity->GetComponent<CharacterComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			const GameEvent* event = inputComponent->ConvertMouse(reinterpret_cast<void*>(&data));
			if (event != nullptr)
			{
				characterComponent->OnCharacterControllerEvent(event);
			}
		}
	}
}