#include "stdafx.h"
#include "DebugInputComponent.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../GameEntity.h"
#include "../Transformation.h"

DebugInputComponent::DebugInputComponent()
{
}


DebugInputComponent::~DebugInputComponent()
{
}

DebugInputComponent* DebugInputComponent::DoClone() const
{
	return new DebugInputComponent(*this);
}

/*
void DebugInputComponent::OnInputEvent(const InputEvent* event)
{
	bool isDirty = false;
	float angle = 0.0f;

	if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_PRESS)
	{
	isDirty = true;
	angle = -2.0f;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
	isDirty = true;
	angle = 2.0f;
	}

	if (isDirty)
	{
		Transformation* transformation = mParent->GetTransformation();
		glm::vec3 rotation = transformation->GetRotation();

		rotation.y += angle;

		transformation->SetRotation(rotation);
	}
}*/
