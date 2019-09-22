#include "stdafx.h"
#include "Player.h"
#include "../components/InputComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/CharacterComponent.h"
#include "../components/ThirdPersonCameraComponent.h"
#include "../components/AnimationComponent.h"

#include "../renderers/IRenderer.h"
#include "../models/Model.h"
#include "../models/animation/AnimatedModel.h"

#include "../GameEvent.h"
#include "../events/characterControllerEvents/BackwardEvent.h"
#include "../events/characterControllerEvents/ForwardEvent.h"
#include "../events/characterControllerEvents/LeftEvent.h"
#include "../events/characterControllerEvents/RightEvent.h"
#include "../events/characterControllerEvents/ShiftEvent.h"
#include "../events/characterControllerEvents/JumpEvent.h"
#include "../events/characterControllerEvents/ZoomEvent.h"
#include "../events/characterControllerEvents/TurnEvent.h"
#include "../Memory.h"
#include "../../utils/serializer/XMLDeserializer.h"


#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player() : 
	BaseGameEntity(),
	mState(IDLE),
	mMaxRunSpeed(0.0f),
	mRunSpeed(0.0f),
	mTurnSpeed(0.0f),
	mUpwardsSpeed(0.0f),
	mCurrentRunSpeed(0.0f),
	mCurrentTurnSpeed(0.0f),
	mCurrentUpwardsSpeed(0.0f),
	mCurrentTurnAngle(0.0f),
	mRotOffsetForLateralMoving(0.0f),
	mHasMoved(false),
	mHasJumped(false)
{
}

Player::Player(	Transformation* transformation, IRenderer* renderer, InputComponent* inputComponent, 
				CharacterComponent* characterComponent, PhysicsComponent* physicsComponent, 
				CollisionComponent* collisionComponent, float runSpeed, float turnSpeed, float upwardsSpeed) :
	BaseGameEntity(transformation, renderer),
mState(IDLE), 
mMaxRunSpeed(runSpeed),
mRunSpeed(runSpeed),
mTurnSpeed(turnSpeed),
mUpwardsSpeed(upwardsSpeed), 
mCurrentRunSpeed(0.0f), 
mCurrentTurnSpeed(0.0f),
mCurrentUpwardsSpeed(0.0f),
mCurrentTurnAngle(0.0f),
mRotOffsetForLateralMoving(0.0f),
mHasMoved(false), 
mHasJumped(false),
mIsShiftPressed(false)
{
	AddComponent(inputComponent);
	AddComponent(physicsComponent);
	AddComponent(collisionComponent);
	AddComponent(characterComponent);
}


Player::~Player()
{
}

void Player::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);
	source->ReadParameter("run_speed", &mMaxRunSpeed);
	mRunSpeed = mMaxRunSpeed;
	source->ReadParameter("turn_speed", &mTurnSpeed);
	source->ReadParameter("upwards_speed", &mUpwardsSpeed);
}

IGameEntity* Player::DoCreate()
{
	return DBG_NEW Player();
}

void Player::Update(float elapsedTime)
{
	BaseGameEntity::Update(elapsedTime);

	mCurrentTurnSpeed = 0.0f;

	UpdateGameEvents();

	std::cout << "velocity = " << mCurrentRunSpeed << " state = " << mState << "\n";

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

void Player::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
	renderSystem->SetCastingShadowsTarget(this);
}

void Player::UpdateAnimations()
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

void Player::UpdateGameEvents()
{
	CharacterComponent* characterComponent = GetComponent<CharacterComponent>();
	if (characterComponent != nullptr)
	{
		while (characterComponent->HasEvents())
		{
			std::shared_ptr<const GameEvent> event = characterComponent->ConsumeEvent();
			switch (mState)
			{
			case IDLE:
			case MOVING:
				if (event->IsOfType<ShiftEvent>())
				{
					std::shared_ptr<const ShiftEvent> shiftEvent = std::static_pointer_cast<const ShiftEvent>(event);
					bool isPressed = shiftEvent->IsPressed();
					UpdateSpeed(isPressed);
				}

				//std::cout << "run speed: " << mRunSpeed << "\n";
				if (event->IsOfType<ForwardEvent>())
				{
					std::shared_ptr<const ForwardEvent> forwardEvent = std::static_pointer_cast<const ForwardEvent>(event);
					bool isPressed = forwardEvent->IsPressed();

					UpdateVelocity(isPressed, true);
				} 
				else if (event->IsOfType<BackwardEvent>())
				{
					std::shared_ptr<const BackwardEvent> backwardEvent = std::static_pointer_cast<const BackwardEvent>(event);
					bool isPressed = backwardEvent->IsPressed();

					UpdateVelocity(isPressed, false);
				}
				
				if (event->IsOfType<LeftEvent>())
				{
					std::shared_ptr<const LeftEvent> leftEvent = std::static_pointer_cast<const LeftEvent>(event);
					bool isPressed = leftEvent->IsPressed();
					UpdateLateralAngle(isPressed, true);					
				}
				else if (event->IsOfType<RightEvent>())
				{
					std::shared_ptr<const RightEvent> rightEvent = std::static_pointer_cast<const RightEvent>(event);
					bool isPressed = rightEvent->IsPressed();
					UpdateLateralAngle(isPressed, false);
				}

				if (event->IsOfType<TurnEvent>())
				{
					std::shared_ptr<const TurnEvent> turnEvent = std::static_pointer_cast<const TurnEvent>(event);
					mHasMoved = true;
					mCurrentTurnSpeed = mTurnSpeed * (mLastTurnX - turnEvent->GetTurn());
					mLastTurnX = turnEvent->GetTurn();
				}
				
				if (event->IsOfType<JumpEvent>())
				{
					mHasMoved = false;
					mHasJumped = true;
					mCurrentUpwardsSpeed = mUpwardsSpeed;
				}

				break;
			default:
				break;
			}
		}
	}
}

void Player::UpdateSpeed(bool isAccelerating)
{
	if (isAccelerating && !mIsShiftPressed)
	{
		mRunSpeed = mMaxRunSpeed * 2.0f;
		mIsShiftPressed = true;
	}
	else if (!isAccelerating && mIsShiftPressed)
	{
		mRunSpeed = mMaxRunSpeed;
		mIsShiftPressed = false;
	}
	if (mCurrentRunSpeed > 0.0f)
	{
		mCurrentRunSpeed = mRunSpeed;
	}
}

void Player::UpdateVelocity(bool isMoving, bool isForward)
{
	mHasMoved |= isMoving;
	if (isMoving)
	{
		float sign = isForward ? 1.0f : -1.0f;
		mCurrentRunSpeed = sign * mRunSpeed;
	}
	else
	{
		mCurrentRunSpeed = 0.0f;
	}
}

void Player::UpdateLateralAngle(bool isMoving, bool isLeft)
{
	if (isMoving)
	{
		float angle = isLeft ? 90.0f : -90.0f;
		mRotOffsetForLateralMoving = glm::radians(angle);
	}
	else
	{
		mRotOffsetForLateralMoving = 0.0f;
	}
	mHasMoved = true;
}

void Player::UpdateIdle(float elapsedTime)
{
	PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
	physicsComponent->SetVelocity(glm::vec3(0.0f));

	CollisionComponent* collisionComponent = GetComponent<CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		bool isOnGround = collisionComponent->IsOnGround();
		if (isOnGround)
		{
			if (mHasJumped)
			{
				mState = JUMPING;
			}
			else if (mHasMoved)
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

			float rotationAngle = CalculateTurnPosition(elapsedTime, mCurrentTurnSpeed);
			glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, rotationAngle, mRotOffsetForLateralMoving, physicsComponent->GetVelocity(), mCurrentRunSpeed);
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
			
		glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, mCurrentTurnAngle, mRotOffsetForLateralMoving, physicsComponent->GetVelocity(), mCurrentRunSpeed);
		newVelocity = CalculateJumpPosition(elapsedTime, newVelocity, mCurrentUpwardsSpeed);
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

glm::vec3 Player::CalculateRunPosition(float elapsedTime, float rotY, float rotOffset, glm::vec3 velocity, float runSpeed)
{
	velocity.x = glm::sin(rotY + rotOffset) * runSpeed;
	velocity.z = glm::cos(rotY + rotOffset) * runSpeed;

	return velocity;
}

float Player::CalculateTurnPosition(float elapsedTime, float turnSpeed)
{
	if (turnSpeed != 0.0f)
	{
		mCurrentTurnAngle += turnSpeed * elapsedTime;
	}
	return mCurrentTurnAngle;
}

glm::vec3 Player::CalculateJumpPosition(float elapsedTime, glm::vec3 velocity, float upwardsSpeed)
{
	if (upwardsSpeed != 0.0f)
	{
		velocity.y = upwardsSpeed;
	}
	return velocity;
}