#include "stdafx.h"
#include "ShiftEvent.h"
#include "GLFW/glfw3.h"
#include "../Memory.h"

ShiftEvent::ShiftEvent() : mIsPressed(false)
{
}

std::shared_ptr<GameEvent> ShiftEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<ShiftEvent> event = std::make_shared<ShiftEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

std::shared_ptr<GameEvent> ShiftEvent::Create()
{
	return std::make_shared<ShiftEvent>();
}

void ShiftEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool ShiftEvent::IsPressed() const
{
	return mIsPressed;
}
