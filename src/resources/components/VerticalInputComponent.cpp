#include "stdafx.h"
#include "VerticalInputComponent.h"

#include <GLFW/glfw3.h>

#include "../GameEntity.h"
#include "../Transformation.h"

VerticalInputComponent::VerticalInputComponent()
{
}


VerticalInputComponent::~VerticalInputComponent()
{
}

VerticalInputComponent* VerticalInputComponent::DoClone() const
{
	return new VerticalInputComponent(*this);
}

/*
void VerticalInputComponent::OnInputEvent(const GameEvent* event)
{
	
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		Transformation* transformation = mParent->GetTransformation();
		glm::vec3 lightPosition = transformation->GetPosition();
		if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			lightPosition.y -= 0.1f;
		}
		else
		{
			lightPosition.y += 0.1f;
		}
		transformation->SetPosition(lightPosition);
	}
}*/
