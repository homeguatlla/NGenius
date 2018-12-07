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

bool InputSystem::HasToBeRegisteredToGameScene() const
{
	return true;
}

bool InputSystem::HasSpecificComponents(const GameEntity* entity) const
{
	return entity != nullptr && entity->HasComponent<InputComponent>() && entity->HasComponent<GameEventsComponent>();
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

BaseVisitable<>::ReturnType InputSystem::Accept(BaseVisitor & guest)
{
	return BaseVisitable<>::ReturnType();
}
