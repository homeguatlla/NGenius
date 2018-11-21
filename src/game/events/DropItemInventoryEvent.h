#pragma once
#include "../../resources/events/CharacterControllerEvent.h"
#include <glm/glm.hpp>

class DropItemInventoryEvent : public CharacterControllerEvent
{
	glm::vec3 mPosition;
	bool mIsPressed;

public:
	DropItemInventoryEvent();
	~DropItemInventoryEvent();

	DropItemInventoryEvent* DoClone(const void* data) const override;

	void SetPosition(const glm::vec3& position);
	const glm::vec3& GetPosition() const;

	void SetPressed(bool pressed);
	bool IsPressed() const;
};

