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
	Release();
	mInputHandler->UnRegisterInputListener(this);
}

void InputSystem::Release()
{
	mEntities.clear();
}

void InputSystem::Update(float elapsedTime)
{
}

void InputSystem::Reload()
{
	Release();
}

bool InputSystem::HasInputComponents(const std::shared_ptr<IGameEntity> entity) const
{
	return entity != nullptr && entity->HasComponent<InputComponent>() && entity->HasComponent<CharacterComponent>();
}

void InputSystem::OnGameEntityAdded(std::shared_ptr<IGameEntity> entity)
{
	if (HasInputComponents(entity))
	{
		AddEntity(entity);
	}
}

void InputSystem::OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity)
{
	if (HasInputComponents(entity))
	{
		RemoveEntity(entity);
	}
}

void InputSystem::AddEntity(std::shared_ptr<IGameEntity> entity)
{
	mEntities.push_back(entity);
}

void InputSystem::RemoveEntity(std::shared_ptr<IGameEntity> entity)
{
	if (HasInputComponents(entity))
	{
		std::vector<std::shared_ptr<IGameEntity>>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](std::shared_ptr<IGameEntity> a) { return a == entity; });
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
	for (auto&& entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		CharacterComponent* characterComponent = entity->GetComponent<CharacterComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			std::shared_ptr<GameEvent> event = inputComponent->ConvertKey(key, action);
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
	for (auto&& entity : mEntities)
	{
		InputComponent* inputComponent = entity->GetComponent<InputComponent>();
		CharacterComponent* characterComponent = entity->GetComponent<CharacterComponent>();
		if (inputComponent != nullptr && characterComponent != nullptr)
		{
			std::shared_ptr<GameEvent> event = inputComponent->ConvertMouse(reinterpret_cast<void*>(&data));
			if (event != nullptr)
			{
				characterComponent->OnCharacterControllerEvent(event);
			}
		}
	}
}