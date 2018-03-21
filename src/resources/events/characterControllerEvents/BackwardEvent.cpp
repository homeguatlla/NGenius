#include "stdafx.h"
#include "BackwardEvent.h"
#include "GLFW/glfw3.h"

BackwardEvent::BackwardEvent() : mIsPressed(false)
{
}


BackwardEvent::~BackwardEvent()
{
}

BackwardEvent* BackwardEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	BackwardEvent* event = new BackwardEvent();
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