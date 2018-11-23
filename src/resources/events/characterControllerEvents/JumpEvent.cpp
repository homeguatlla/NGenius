#include "stdafx.h"
#include "JumpEvent.h"
#include <GLFW/glfw3.h>

JumpEvent::JumpEvent()
{
}


JumpEvent::~JumpEvent()
{
}

JumpEvent* JumpEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	JumpEvent* event = new JumpEvent();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}


void JumpEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool JumpEvent::IsPressed() const
{
	return mIsPressed;
}