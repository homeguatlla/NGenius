#include "stdafx.h"
#include "Character.h"
#include "../components/InputComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/GameEventsComponent.h"
#include "../components/ThirdPersonCameraComponent.h"
#include "../components/AnimationComponent.h"

#include "../renderers/IRenderer.h"
#include "../models/Model.h"
#include "../models/animation/AnimatedModel.h"

#include "../GameEvent.h"
#include "../events/characterControllerEvents/BackwardEvent.h"
#include "../events/characterControllerEvents/ForwardEvent.h"
#include "../events/characterControllerEvents/JumpEvent.h"
#include "../events/characterControllerEvents/ZoomEvent.h"
#include "../events/characterControllerEvents/TurnEvent.h"


#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Character::Character(	Transformation* transformation, IRenderer* renderer, InputComponent* inputComponent, 
				GameEventsComponent* characterComponent, PhysicsComponent* physicsComponent, 
				CollisionComponent* collisionComponent, float runSpeed, float turnSpeed, float upwardsSpeed) :
GameEntity(transformation, renderer), 
mState(IDLE), 
mRunSpeed(runSpeed), 
mTurnSpeed(turnSpeed),
mUpwardsSpeed(upwardsSpeed), 
mCurrentRunSpeed(0.0f), 
mCurrentTurnSpeed(0.0f),
mCurrentUpwardsSpeed(0.0f),
mCurrentTurnAngle(0.0f),
mOriginalTurnAngle(0.0f),
mHasMoved(false), 
mHasJumped(false)
{
	AddComponent(inputComponent);
	AddComponent(physicsComponent);
	AddComponent(collisionComponent);
	AddComponent(characterComponent);
	//saving the original rotation Y because player can modify it when rotating the player and 
	//we need to perserve the original rotation
	mOriginalTurnAngle = transformation->GetRotation().y;
}


Character::~Character()
{
}

void Character::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);

	mCurrentTurnSpeed = 0.0f;

	UpdateGameEvents();

	switch (mState)
	{
		case IDLE:
			UpdateIdle(elapsedTime);
			break;
		case MOVING:
			UpdateMoving(elapsedTime);
			break;
		case JUMPING:
			UpdateJumping(elapsedTime);
			break;
		case FALLING:
 			UpdateFalling(elapsedTime);
			break;
		default:
			break;
	}

	UpdateAnimations();
	//PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
	//std::cout << "state: " << mState << " velocity = " << physicsComponent->GetVelocity().y << "\n";
}

void Character::UpdateAnimations()
{
	AnimationComponent* animationComponent = GetComponent<AnimationComponent>();
	if (animationComponent != nullptr)
	{
		if (mState == MOVING && glm::abs(mCurrentRunSpeed) > 0.0f)
		{
			animationComponent->PlayAnimation("animation_0");
		}
		else
		{
			animationComponent->StopAnimation();
		}
	}
}

void Character::UpdateGameEvents()
{
	GameEventsComponent* characterComponent = GetComponent<GameEventsComponent>();

	characterComponent->StartIterate();
	while (characterComponent->HasEvents())
	{
		const GameEvent* event = characterComponent->GetEvent();
		switch (mState)
		{
		case IDLE:
		case MOVING:
			if (event->IsOfType<ForwardEvent>())
			{
				const ForwardEvent* forwardEvent = static_cast<const ForwardEvent*>(event);
				bool isPressed = forwardEvent->IsPressed();
				
				mHasMoved |= isPressed;

				if (isPressed)
				{
					mCurrentRunSpeed = mRunSpeed;
				}
				else
				{
					mCurrentRunSpeed = 0.0f;
				}
				characterComponent->ConsumeEvent();
			}
			else if (event->IsOfType<BackwardEvent>())
			{
				const BackwardEvent* backwardEvent = static_cast<const BackwardEvent*>(event);
				bool isPressed = backwardEvent->IsPressed();

				mHasMoved |= isPressed;

				if (isPressed)
				{
					mCurrentRunSpeed = -mRunSpeed;
				}
				else
				{
					mCurrentRunSpeed = 0.0f;
				}
				characterComponent->ConsumeEvent();
			}
			else if (event->IsOfType<TurnEvent>())
			{
				const TurnEvent* turnEvent = static_cast<const TurnEvent*>(event);
				mHasMoved = true;
				mCurrentTurnSpeed = mTurnSpeed * (mLastTurnX - turnEvent->GetTurn());
				mLastTurnX = turnEvent->GetTurn();
				characterComponent->ConsumeEvent();
			}
			else if (event->IsOfType<JumpEvent>())
			{
				const JumpEvent* jumpEvent = static_cast<const JumpEvent*>(event);
				if (jumpEvent->IsPressed())
				{
					mHasMoved = false;
					mHasJumped = true;
					mCurrentUpwardsSpeed = mUpwardsSpeed;
				}
				characterComponent->ConsumeEvent();
			} 
		break;
		default:
			break;
		}
	}
}

void Character::UpdateIdle(float elapsedTime)
{
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();
	
	PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
	physicsComponent->SetVelocity(glm::vec3(0.0f));

	if (isOnGround)
	{
		if (mHasJumped)
		{
			mState = JUMPING;
		}
		else if(mHasMoved)
		{
			mState = MOVING;
		}
		else
		{
			mCurrentRunSpeed = 0.0f;
			mCurrentTurnSpeed = 0.0f;
			mCurrentUpwardsSpeed = 0.0f;
		}
	}
	else if (!isOnGround)
	{
		mState = FALLING;
	}
}

void Character::UpdateMoving(float elapsedTime)
{
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();

	if (isOnGround)
	{
		if (mHasJumped)
		{
			mState = JUMPING;
		}
		else if (!mHasMoved)
		{
			mState = IDLE;
		}
		else
		{
			Transformation* transformation = GetTransformation();
			PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();

			float rotationAngle = CalculateTurnPosition(elapsedTime, mCurrentTurnSpeed);
			rotationAngle += mOriginalTurnAngle;
			glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, rotationAngle, physicsComponent->GetVelocity(), mCurrentRunSpeed);
			newVelocity = CalculateJumpPosition(elapsedTime, newVelocity, mCurrentUpwardsSpeed);
			physicsComponent->SetVelocity(newVelocity);

			glm::vec3 rotation = transformation->GetRotation();
			rotation.y = mCurrentTurnAngle;
			transformation->SetRotation(rotation);
		}
	}
	else if (!isOnGround)
	{
		mState = FALLING;
	}
}

void Character::UpdateJumping(float elapsedTime)
{
	Transformation* transformation = GetTransformation();
		
	float groundHeight = GetComponent<CollisionComponent>()->GetGroundHeight();
	if (transformation->GetPosition().y - groundHeight > 0.5f)
	{
		mHasJumped = false;
        mState = FALLING;
		mCurrentUpwardsSpeed = 0.0f;
	}
	else
	{
		PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
			
		glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, mCurrentTurnAngle, physicsComponent->GetVelocity(), mCurrentRunSpeed);
		newVelocity = CalculateJumpPosition(elapsedTime, newVelocity, mCurrentUpwardsSpeed);
		physicsComponent->SetVelocity(newVelocity);
	}
}

void Character::UpdateFalling(float elapsedTime)
{
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();

	if (isOnGround)
	{
		mState = IDLE;
	}
}

glm::vec3 Character::CalculateRunPosition(float elapsedTime, float rotY, glm::vec3 velocity, float runSpeed)
{
	velocity.x = glm::sin(rotY) * runSpeed;
	velocity.z = glm::cos(rotY) * runSpeed;

	return velocity;
}

float Character::CalculateTurnPosition(float elapsedTime, float turnSpeed)
{
	if (turnSpeed != 0.0f)
	{
		mCurrentTurnAngle += turnSpeed * elapsedTime;
	}
	return mCurrentTurnAngle;
}

glm::vec3 Character::CalculateJumpPosition(float elapsedTime, glm::vec3 velocity, float upwardsSpeed)
{
	if (upwardsSpeed != 0.0f)
	{
		velocity.y = upwardsSpeed;
	}
	return velocity;
}