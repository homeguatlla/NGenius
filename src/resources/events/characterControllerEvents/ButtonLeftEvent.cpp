#include "stdafx.h"
#include "ButtonLeftEvent.h"
#include "../../systems/InputSystem.h"
#include <GLFW/glfw3.h>

ButtonLeftEvent::ButtonLeftEvent() : mIsPressed(false)
{
}

ButtonLeftEvent::~ButtonLeftEvent()
{
}

std::shared_ptr<GameEvent> ButtonLeftEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	std::shared_ptr<ButtonLeftEvent> buttonLeftEvent = std::make_shared<ButtonLeftEvent>();
	buttonLeftEvent->SetAction(mouseData->mAction);

	return buttonLeftEvent;
}

std::shared_ptr<GameEvent> ButtonLeftEvent::Create()
{
	return std::make_shared<ButtonLeftEvent>();
}

void ButtonLeftEvent::SetAction(int action)
{
	mIsPressed = action == GLFW_PRESS;
}

bool ButtonLeftEvent::IsPressed() const
{
	return mIsPressed;
}

bool ButtonLeftEvent::IsReleased() const
{
	return !mIsPressed;
}