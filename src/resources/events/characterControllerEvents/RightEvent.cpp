#include "stdafx.h"
#include "RightEvent.h"
#include "GLFW/glfw3.h"
#include "../Memory.h"

RightEvent::RightEvent() : mIsPressed(false)
{
}

std::shared_ptr<GameEvent> RightEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<RightEvent> event = std::make_shared<RightEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

std::shared_ptr<GameEvent> RightEvent::Create()
{
	return std::make_shared<RightEvent>();
}

void RightEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool RightEvent::IsPressed() const
{
	return mIsPressed;
}
