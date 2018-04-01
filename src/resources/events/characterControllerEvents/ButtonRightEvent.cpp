#include "stdafx.h"
#include "ButtonRightEvent.h"
#include "../../systems/InputSystem.h"
#include <GLFW/glfw3.h>

ButtonRightEvent::ButtonRightEvent() : mIsPressed(false)
{
}

ButtonRightEvent::~ButtonRightEvent()
{
}

ButtonRightEvent* ButtonRightEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	ButtonRightEvent* buttonRightEvent = new ButtonRightEvent();
	buttonRightEvent->SetAction(mouseData->mAction);

	return buttonRightEvent;
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