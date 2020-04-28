#include "stdafx.h"
#include "JumpEvent.h"
#include "GLFW/glfw3.h"

std::shared_ptr<GameEvent> JumpEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	std::shared_ptr<JumpEvent> event = std::make_shared<JumpEvent>();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

std::shared_ptr<GameEvent> JumpEvent::Create()
{
	return std::make_shared<JumpEvent>();
}
