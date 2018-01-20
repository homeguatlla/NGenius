#include "stdafx.h"
#include "PlayerInputComponent.h"
#include <GLFW/glfw3.h>

#include "../GameEntity.h"
#include "../Transformation.h"

PlayerInputComponent::PlayerInputComponent(GLFWwindow* window, float runSpeed, float turnSpeed, float upwardsSpeed) :
IInputComponent(window),
mRunSpeed(runSpeed),
mTurnSpeed(turnSpeed),
mUpwardsSpeed(upwardsSpeed),
mHasMoved(false),
mHasJumped(false)
{
}


PlayerInputComponent::~PlayerInputComponent()
{
}

PlayerInputComponent* PlayerInputComponent::DoClone() const
{
	return new PlayerInputComponent(*this);
}

float PlayerInputComponent::GetCurrentRunSpeed() const
{
	return mCurrentRunSpeed;
}

float PlayerInputComponent::GetCurrentTurnSpeed() const
{
	return mCurrentTurnSpeed;
}

float PlayerInputComponent::GetCurrentUpwardsSpeed() const
{
	return mCurrentUpwardsSpeed;
}

bool PlayerInputComponent::HasMoved() const
{
	return mHasMoved;
}

bool PlayerInputComponent::HasJumped() const
{
	return mHasJumped;
}

void PlayerInputComponent::Update(float elapsedTime)
{
	mHasMoved = UpdateTurnSpeed(elapsedTime);
	mHasMoved |= UpdateRunSpeed(elapsedTime);
	mHasMoved |= UpdateUpwardsSpeed(elapsedTime);
}

bool PlayerInputComponent::UpdateTurnSpeed(float elapsedTime)
{
	bool moved = true;

	if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		mCurrentTurnSpeed = mTurnSpeed;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		mCurrentTurnSpeed = -mTurnSpeed;
	}
	else
	{
		mCurrentTurnSpeed = 0.0f;
		moved = false;
	}
	return moved;
}

bool PlayerInputComponent::UpdateRunSpeed(float elapsedTime)
{
	bool moved = true;

	if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mCurrentRunSpeed = mRunSpeed;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mCurrentRunSpeed = -mRunSpeed;
	}
	else
	{
		mCurrentRunSpeed = 0.0f;
		moved = false;
	}

	return moved;
}

bool PlayerInputComponent::UpdateUpwardsSpeed(float elapsedTime)
{
	if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mCurrentUpwardsSpeed = mUpwardsSpeed;
		mHasJumped = true;
	}
	else                      
	{
		mCurrentUpwardsSpeed  = 0.0f;
		mHasJumped = false;
	}
	
	return mHasJumped;
}
