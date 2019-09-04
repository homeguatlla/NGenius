#include "stdafx.h"
#include "ButtonRightEvent.h"
#include "../../systems/InputSystem.h"
#include <GLFW/glfw3.h>
#include "../Memory.h"

ButtonRightEvent::ButtonRightEvent() : mIsPressed(false)
{
}

ButtonRightEvent::~ButtonRightEvent()
{
}

std::shared_ptr<GameEvent> ButtonRightEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	std::shared_ptr<ButtonRightEvent> buttonRightEvent = std::make_shared<ButtonRightEvent>();
	buttonRightEvent->SetAction(mouseData->mAction);

	return buttonRightEvent;
}

std::shared_ptr<GameEvent> ButtonRightEvent::Create()
{
	return std::make_shared<ButtonRightEvent>();
}

void ButtonRightEvent::SetAction(int action)
{
	mIsPressed = action == GLFW_PRESS;
}

bool ButtonRightEvent::IsPressed() const
{
	return mIsPressed;
}

bool ButtonRightEvent::IsReleased() const
{
	return !mIsPressed;
}