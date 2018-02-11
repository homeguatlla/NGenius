#include "stdafx.h"
#include "Player.h"
#include "../components/PlayerInputComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"

#include <iostream>

Player::Player(Transformation* transformation, IRenderer_* renderer, PlayerInputComponent* playerInputComponent, PhysicsComponent* physicsComponent, CollisionComponent* collisionComponent) :
GameEntity(transformation, renderer), mState(IDLE)
{
	AddComponent(playerInputComponent);
	AddComponent(physicsComponent);
	AddComponent(collisionComponent);
}


Player::~Player()
{
}

void Player::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);

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
		case FLYING:
			UpdateFlying(elapsedTime);
			break;
		case FALLING:
 			UpdateFalling(elapsedTime);
			break;
		default:
			break;
	}
	/*PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
	std::cout << "velocity = " << physicsComponent->GetVelocity().y << "\n";*/
}

void Player::UpdateIdle(float elapsedTime)
{
	PlayerInputComponent* playerInputComponent = GetComponent<PlayerInputComponent>();
	bool isMoving = playerInputComponent->HasMoved();
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();
	
	PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
	physicsComponent->SetVelocity(glm::vec3(0.0f));

	if (isMoving && isOnGround)
	{
		if (playerInputComponent->HasJumped())
		{
			mState = JUMPING;
		}
		else
		{
			mState = MOVING;
		}
	}
	else if (!isOnGround)
	{
		mState = FLYING;
	}
}

void Player::UpdateMoving(float elapsedTime)
{
	PlayerInputComponent* playerInputComponent = GetComponent<PlayerInputComponent>();
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();

	if (!playerInputComponent->HasMoved())
	{
		mState = IDLE;
	}
	else if (!isOnGround)
	{
		mState = FLYING;
	}
	else
	{
		Transformation* transformation = GetTransformation();
		PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
		CalculateTurnPosition(elapsedTime, transformation, playerInputComponent->GetCurrentTurnSpeed());

		glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, transformation, physicsComponent->GetVelocity(), playerInputComponent->GetCurrentRunSpeed());
		newVelocity = CalculateJumpPosition(elapsedTime, transformation, newVelocity, playerInputComponent->GetCurrentUpwardsSpeed());
		physicsComponent->SetVelocity(newVelocity);
	}
}

void Player::UpdateFlying(float elapsdeTime)
{
	CollisionComponent* collisionComponent = GetComponent<CollisionComponent>();

	if (collisionComponent->IsOnGround())
	{
		mState = IDLE;
	}
}

void Player::UpdateJumping(float elapsedTime)
{
	PlayerInputComponent* playerInputComponent = GetComponent<PlayerInputComponent>();

	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();

	if (isOnGround && !playerInputComponent->HasMoved())
	{
		mState = IDLE;
	}
	else
	{
		Transformation* transformation = GetTransformation();
		
		float groundHeight = GetComponent<CollisionComponent>()->GetGroundHeight();
		if (transformation->GetPosition().y - groundHeight > 0.5f)
		{
          	mState = FALLING;
		}
		else
		{
			Transformation* transformation = GetTransformation();
			PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
			
			glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, transformation, physicsComponent->GetVelocity(), playerInputComponent->GetCurrentRunSpeed());
			newVelocity = CalculateJumpPosition(elapsedTime, transformation, newVelocity, playerInputComponent->GetCurrentUpwardsSpeed());
			physicsComponent->SetVelocity(newVelocity);
		}
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