#include "stdafx.h"
#include "ButtonLeftEvent.h"
#include "../../systems/InputSystem.h"
#include <GLFW/glfw3.h>
#include "../Memory.h"

ButtonLeftEvent::ButtonLeftEvent() : mIsPressed(false)
{
}

ButtonLeftEvent::~ButtonLeftEvent()
{
}

ButtonLeftEvent* ButtonLeftEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	ButtonLeftEvent* buttonLeftEvent = DBG_NEW ButtonLeftEvent();
	buttonLeftEvent->SetAction(mouseData->mAction);

	return buttonLeftEvent;
}

GameEvent* ButtonLeftEvent::Create()
{
	return DBG_NEW ButtonLeftEvent();
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