#include "stdafx.h"
#include "BackwardEvent.h"
#include "GLFW/glfw3.h"

std::shared_ptr<GameEvent> BackwardEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<BackwardEvent> event = std::make_shared<BackwardEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

std::shared_ptr<GameEvent> BackwardEvent::Create()
{
	return std::make_shared<BackwardEvent>();
}
