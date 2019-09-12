#include "stdafx.h"
#include "LeftEvent.h"
#include "GLFW/glfw3.h"
#include "../Memory.h"

LeftEvent::LeftEvent() : mIsPressed(false)
{
}

std::shared_ptr<GameEvent> LeftEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<LeftEvent> event = std::make_shared<LeftEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

std::shared_ptr<GameEvent> LeftEvent::Create()
{
	return std::make_shared<LeftEvent>();
}

void LeftEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool LeftEvent::IsPressed() const
{
	return mIsPressed;
}
