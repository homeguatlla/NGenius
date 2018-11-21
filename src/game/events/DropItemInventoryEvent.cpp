#include "stdafx.h"
#include "DropItemInventoryEvent.h"
#include <GLFW/glfw3.h>

DropItemInventoryEvent::DropItemInventoryEvent()
{
}


DropItemInventoryEvent::~DropItemInventoryEvent()
{
}

void DropItemInventoryEvent::SetPosition(const glm::vec3& position)
{
	mPosition = position;
}

const glm::vec3& DropItemInventoryEvent::GetPosition() const
{
	return mPosition;
}

DropItemInventoryEvent* DropItemInventoryEvent::DoClone(const void* data) const
{
	const int* action = reinterpret_cast<const int*>(data);
	DropItemInventoryEvent* event = new DropItemInventoryEvent();
	event->SetPressed(action != GLFW_RELEASE);
	return event;
}

void DropItemInventoryEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool DropItemInventoryEvent::IsPressed() const
{
	return mIsPressed;
}
