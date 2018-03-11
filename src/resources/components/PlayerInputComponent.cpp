#include "stdafx.h"
#include "PlayerInputComponent.h"
#include "../GameEntity.h"
#include "../Transformation.h"

#include <GLFW/glfw3.h>
#include <functional>

PlayerInputComponent::PlayerInputComponent(float runSpeed, float turnSpeed, float upwardsSpeed) :
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

void PlayerInputComponent::UpdateTurnSpeed(int key, int action)
{
	bool moved = true;
	
	if (key == GLFW_KEY_A)
	{
		mCurrentTurnSpeed = mTurnSpeed;
	}
	else if (key == GLFW_KEY_D)
	{
		mCurrentTurnSpeed = -mTurnSpeed;
	}

	if (action == GLFW_RELEASE)
	{
		mCurrentTurnSpeed = 0.0f;
		moved = false;
	}

	mHasMoved |= moved;
}

void PlayerInputComponent::UpdateRunSpeed(int key, int action)
{
	bool moved = true;
	
	if (key == GLFW_KEY_W)
	{
		mCurrentRunSpeed = mRunSpeed;
	}
	else if (key == GLFW_KEY_S)
	{
		mCurrentRunSpeed = -mRunSpeed;
	}
	
	if(action == GLFW_RELEASE)
	{
		mCurrentRunSpeed = 0.0f;
		moved = false;
	}
	
	mHasMoved |= moved;
}

void PlayerInputComponent::UpdateUpwardsSpeed(int action)
{
	if (action == GLFW_PRESS)
	{
		mCurrentUpwardsSpeed = mUpwardsSpeed;
		mHasJumped = true;
	}
	else                      
	{
		mCurrentUpwardsSpeed  = 0.0f;
		mHasJumped = false;
	}
}

void PlayerInputComponent::OnKey(int key, int action)
{
	switch (key)
	{
		case GLFW_KEY_SPACE:
			UpdateUpwardsSpeed(action);
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			UpdateRunSpeed(key, action);
		break;
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			UpdateTurnSpeed(key, action);
			break;
		default:
			break;
	}
}
