#include "stdafx.h"
#include "PlayerInputComponent.h"
#include "../GameEntity.h"
#include "../Transformation.h"
#include "../../input/InputEvent.h"
#include "../../input/events/ForwardEvent.h"

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

void PlayerInputComponent::UpdateRunSpeed(bool isForward)
{
	bool moved = true;
	
	if (isForward)
	{
		mCurrentRunSpeed = mRunSpeed;
	}
	else
	{
		mCurrentRunSpeed = -mRunSpeed;
	}
	
	/*if(action == GLFW_RELEASE)
	{
		mCurrentRunSpeed = 0.0f;
		moved = false;
	}*/
	
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

void PlayerInputComponent::OnInputEvent(const InputEvent* event)
{
	if (event->IsOfType<ForwardEvent>())
	{
		UpdateRunSpeed(1);
	}
	/*
	switch (event->GetKey())
	{
		case GLFW_KEY_SPACE:
			UpdateUpwardsSpeed(event->GetAction());
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			UpdateRunSpeed(event->GetKey(), event->GetAction());
		break;
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			UpdateTurnSpeed(event->GetKey(), event->GetAction());
			break;
		default:
			break;
	}*/
}
