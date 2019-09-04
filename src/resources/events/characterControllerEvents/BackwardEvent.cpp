#include "stdafx.h"
#include "BackwardEvent.h"
#include "GLFW/glfw3.h"

BackwardEvent::BackwardEvent() : mIsPressed(false)
{
}


BackwardEvent::~BackwardEvent()
{
}

std::shared_ptr<GameEvent> BackwardEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<BackwardEvent> event = std::make_shared<BackwardEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

void BackwardEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool BackwardEvent::IsPressed() const
{
	return mIsPressed;
}

std::shared_ptr<GameEvent> BackwardEvent::Create()
{
	return std::make_shared<BackwardEvent>();
}
