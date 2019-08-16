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

std::shared_ptr<GameEvent> ForwardEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<ForwardEvent> event = std::make_shared<ForwardEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

std::shared_ptr<GameEvent> ForwardEvent::Create()
{
	return std::make_shared<ForwardEvent>();
}

void ForwardEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool ForwardEvent::IsPressed() const
{
	return mIsPressed;
}
