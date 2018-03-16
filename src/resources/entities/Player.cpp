#include "stdafx.h"
#include "Player.h"
#include "../components/InputComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/CharacterComponent.h"
#include "../GameEvent.h"
#include "../events/characterControllerEvents/BackwardEvent.h"
#include "../events/characterControllerEvents/ForwardEvent.h"
#include "../events/characterControllerEvents/StopEvent.h"
#include "../events/characterControllerEvents/JumpEvent.h"

#include <iostream>

Player::Player(	Transformation* transformation, IRenderer* renderer, InputComponent* inputComponent, 
				CharacterComponent* characterComponent, PhysicsComponent* physicsComponent, 
				CollisionComponent* collisionComponent, float runSpeed, float upwardsSpeed) :
GameEntity(transformation, renderer), 
mState(IDLE), 
mRunSpeed(runSpeed), 
mUpwardsSpeed(upwardsSpeed), 
mCurrentRunSpeed(0.0f), 
mCurrentUpwardsSpeed(0.0f), 
mHasMoved(false), 
mHasJumped(false)
{
	AddComponent(inputComponent);
	AddComponent(physicsComponent);
	AddComponent(collisionComponent);
	AddComponent(characterComponent);
}


Player::~Player()
{
}

void Player::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);

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
	PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
	std::cout << "state: " << mState << " velocity = " << physicsComponent->GetVelocity().y << "\n";
}

void Player::UpdateGameEvents()
{
	CharacterComponent* characterComponent = GetComponent<CharacterComponent>();
	while (characterComponent->HasEvents())
	{
		const GameEvent* event = characterComponent->ConsumeEvent();
		switch (mState)
		{
		case IDLE:
		case MOVING:
			if (event->IsOfType<ForwardEvent>())
			{
				mHasMoved = true;
				mCurrentRunSpeed = mRunSpeed;

			}
			else if (event->IsOfType<BackwardEvent>())
			{
				mHasMoved = true;
				mCurrentRunSpeed = -mRunSpeed;
			}
			else if (event->IsOfType<JumpEvent>())
			{
				mHasMoved = false;
				mHasJumped = true;
				mCurrentUpwardsSpeed = mUpwardsSpeed;
			}
			else if(event->IsOfType<StopEvent>())
			{
				mHasMoved = false;
				mCurrentRunSpeed = 0.0f;
			}
		break;
		default:
			break;
		}
	}
}

void Player::UpdateIdle(float elapsedTime)
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
	}
	else if (!isOnGround)
	{
		mState = FALLING;
	}
}

void Player::UpdateMoving(float elapsedTime)
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

			glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, transformation, physicsComponent->GetVelocity(), mCurrentRunSpeed);
			newVelocity = CalculateJumpPosition(elapsedTime, transformation, newVelocity, mCurrentUpwardsSpeed);
			physicsComponent->SetVelocity(newVelocity);
		}
	}
	else if (!isOnGround)
	{
		mState = FALLING;
	}
}

void Player::UpdateJumping(float elapsedTime)
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
			
		glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, transformation, physicsComponent->GetVelocity(), mCurrentRunSpeed);
		newVelocity = CalculateJumpPosition(elapsedTime, transformation, newVelocity, mCurrentUpwardsSpeed);
		physicsComponent->SetVelocity(newVelocity);
	}
}

void Player::UpdateFalling(float elapsedTime)
{
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();

	if (isOnGround)
	{
		mState = IDLE;
	}
}

glm::vec3 Player::CalculateRunPosition(float elapsedTime, Transformation* transformation, glm::vec3 velocity, float runSpeed)
{
	float rotY = transformation->GetRotation().y;
	velocity.x = glm::sin(rotY) * runSpeed;
	velocity.z = glm::cos(rotY) * runSpeed;

	return velocity;
}

void Player::CalculateTurnPosition(float elapsedTime, Transformation* transformation, float turnSpeed)
{
	if (turnSpeed != 0.0f)
	{
		glm::vec3 rotation = transformation->GetRotation();
		rotation.y += turnSpeed * elapsedTime;
		transformation->SetRotation(rotation);
	}
}

glm::vec3 Player::CalculateJumpPosition(float elapsedTime, Transformation* transformation, glm::vec3 velocity, float upwardsSpeed)
{
	if (upwardsSpeed != 0.0f)
	{
		velocity.y = upwardsSpeed;
	}
	return velocity;
}