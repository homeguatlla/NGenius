#include "stdafx.h"
#include "ForwardEvent.h"
#include "GLFW/glfw3.h"

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
