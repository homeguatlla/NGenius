#include "stdafx.h"
#include "ForwardEvent.h"
#include "GLFW/glfw3.h"
#include "../Memory.h"

ForwardEvent::ForwardEvent() : mIsPressed(false)
{
}


ForwardEvent::~ForwardEvent()
{
}

ForwardEvent* ForwardEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	ForwardEvent* event = DBG_NEW ForwardEvent();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

void ForwardEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool ForwardEvent::IsPressed() const
{
	return mIsPressed;
}
