#include "stdafx.h"
#include "Player.h"
#include "src/resources/entities/Player/PlayerFsm/states/Idle.h"
#include "src/resources/entities/Player/PlayerFsm/states/Run.h"
#include "src/resources/entities/Player/PlayerFsm/states/Jump.h"
#include "src/resources/entities/Player/PlayerFsm/states/OnAir.h"

#include "src/resources/entities/Player/PlayerFsm/transitions/EnterIdle.h"
#include "src/resources/entities/Player/PlayerFsm/transitions/EnterRun.h"
#include "src/resources/entities/Player/PlayerFsm/transitions/EnterJump.h"
#include "src/resources/entities/Player/PlayerFsm/transitions/EnterOnAir.h"

#include "src/resources/components/InputComponent.h"
#include "src/resources/components/CollisionComponent.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/CharacterComponent.h"
#include "src/resources/components/ThirdPersonCameraComponent.h"
#include "src/resources/components/AnimationComponent.h"

#include "src/resources/events/characterControllerEvents/TurnEvent.h"
#include "src/resources/events/characterControllerEvents/ForwardEvent.h"
#include "src/resources/events/characterControllerEvents/BackwardEvent.h"
#include "src/resources/events/characterControllerEvents/JumpEvent.h"

/*
#include "src/resources/renderers/IRenderer.h"
#include "src/resources/models/Model.h"
#include "src/resources/models/animation/AnimatedModel.h"

#include "src/resources/GameEvent.h"
#include "src/resources/events/characterControllerEvents/LeftEvent.h"
#include "src/resources/events/characterControllerEvents/RightEvent.h"
#include "src/resources/events/characterControllerEvents/ShiftEvent.h"
#include "src/resources/events/characterControllerEvents/ZoomEvent.h"
#include "Memory.h"
#include "src/utils/serializer/XMLDeserializer.h"
#include "GameConstants.h"
#include "source/utils/Math.h"


#include <glm/gtc/matrix_transform.hpp>*/
#include <iostream>

Player::Player() : 
	BaseGameEntity(),
	mMaxRunSpeed{ 0.0f },
	mMaxTurnSpeed{ 0.0f },
	mMaxUpwardsSpeed{ 0.0f },
	mCurrentTurnSpeed{ 0.0f },
	mLastTurnX{ 0.0f },
	mCurrentTurnAngle{ 0.0f },
	mRunSpeed {mMaxRunSpeed},
	mCurrentRunSpeed {0.0f}
{
}

Player::Player(	Transformation* transformation, IRenderer* renderer, InputComponent* inputComponent, 
				CharacterComponent* characterComponent, PhysicsComponent* physicsComponent, 
				CollisionComponent* collisionComponent, float runSpeed, float turnSpeed, float upwardsSpeed) :
	BaseGameEntity(transformation, renderer), 
	mMaxRunSpeed{ runSpeed },
	mMaxTurnSpeed{ turnSpeed },
	mMaxUpwardsSpeed{ upwardsSpeed },
	mCurrentTurnSpeed{ 0.0f },
	mLastTurnX{ 0.0f },
	mCurrentTurnAngle{ 0.0f },
	mRunSpeed{ mMaxRunSpeed },
	mCurrentRunSpeed{ 0.0f }
{
	AddComponent(inputComponent);
	AddComponent(physicsComponent);
	AddComponent(collisionComponent);
	AddComponent(characterComponent);
	mAnimationComponent = std::shared_ptr<AnimationComponent>(GetComponent<AnimationComponent>());
	mCollisionComponent = std::shared_ptr<CollisionComponent>(collisionComponent);
	mPhysicsComponent = std::shared_ptr<PhysicsComponent>(physicsComponent);
	mCharacterComponent = std::shared_ptr<CharacterComponent>(characterComponent);
}

std::shared_ptr<IGameEntity> Player::DoCreate()
{
	return std::make_shared<Player>();
}

void Player::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
	mAnimationComponent = std::shared_ptr<AnimationComponent>(GetComponent<AnimationComponent>());
	mCollisionComponent = std::shared_ptr<CollisionComponent>(GetComponent<CollisionComponent>());
	mPhysicsComponent = std::shared_ptr<PhysicsComponent>(GetComponent<PhysicsComponent>());
	mCharacterComponent = std::shared_ptr<CharacterComponent>(GetComponent<CharacterComponent>());
	CreateStatesMachine();
	//renderSystem->SetCastingShadowsTarget(this);
}

void Player::Update(float elapsedTime)
{
	BaseGameEntity::Update(elapsedTime);

	UpdateEvents(elapsedTime);
	mStatesMachine->Update(elapsedTime);

	//std::cout << "velocity = " << mCurrentRunSpeed << " state = " << mState << "\n";
	//std::cout << "state: " << static_cast<int>(mStatesMachine->GetCurrentState()->GetID()) << std::endl;
}

void Player::UpdateEvents(float deltaTime)
{
	while (mCharacterComponent->HasEvents())
	{
		if (mCharacterComponent->IsNextEventOfType<TurnEvent>())
		{
			TreatTurnEvent(deltaTime, mCharacterComponent->ConsumeEvent());
		}
		else if (mCharacterComponent->IsNextEventOfType<ForwardEvent>() || mCharacterComponent->IsNextEventOfType<BackwardEvent>())
		{
			TreatMoveEvent(deltaTime, mCharacterComponent->ConsumeEvent());
		}
		else if (mCharacterComponent->IsNextEventOfType<JumpEvent>())
		{
			TreatJumpEvent(deltaTime, mCharacterComponent->ConsumeEvent());
		}
		else
		{
			mCharacterComponent->ConsumeEvent();
		}
	}
}

void Player::PerformMovement(float elapsedTime)
{
	auto transformation = GetTransformation();
	glm::vec3 rotation = transformation->GetRotation();
	glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, rotation.y, 0, mPhysicsComponent->GetVelocity(), mCurrentRunSpeed);
	mPhysicsComponent->SetVelocity(newVelocity);
}

void Player::PerformJump(float elapsedTime)
{
	auto velocity = mPhysicsComponent->GetVelocity();

	velocity.y += mMaxUpwardsSpeed;

	mPhysicsComponent->SetVelocity(velocity);
}


void Player::CreateStatesMachine()
{
	mPlayerContext = std::make_shared<PlayerContext>(std::static_pointer_cast<Player>(shared_from_this()));
	mStatesMachine = std::make_unique<core::utils::FSM::StatesMachine<PlayerState, PlayerContext>>(mPlayerContext);

	auto idle = std::make_shared<Idle>();
	auto run = std::make_shared<Run>();
	auto jump = std::make_shared<Jump>();
	auto onAir = std::make_shared<OnAir>();
	
	mStatesMachine->AddState(idle);
	mStatesMachine->AddState(run);
	mStatesMachine->AddState(jump);
	mStatesMachine->AddState(onAir);
	
	//from Idle
	mStatesMachine->AddTransition(std::make_unique<EnterRun>(idle, run));
	mStatesMachine->AddTransition(std::make_unique<EnterJump>(idle, jump));
	mStatesMachine->AddTransition(std::make_unique<EnterOnAir>(idle, onAir));

	//from Run
	mStatesMachine->AddTransition(std::make_unique<EnterIdle>(run, idle));
	mStatesMachine->AddTransition(std::make_unique<EnterJump>(run, jump));
	mStatesMachine->AddTransition(std::make_unique<EnterOnAir>(run, onAir));

	//from Jump
	mStatesMachine->AddTransition(std::make_unique<EnterOnAir>(jump, onAir));

	//from OnAir
	mStatesMachine->AddTransition(std::make_unique<EnterIdle>(onAir, idle));
	mStatesMachine->AddTransition(std::make_unique<EnterRun>(onAir, run));

	mStatesMachine->SetInitialState(idle->GetID());
}

void Player::TreatTurnEvent(float elapsedTime, std::shared_ptr<GameEvent> event)
{
	std::shared_ptr<const TurnEvent> turnEvent = std::static_pointer_cast<const TurnEvent>(event);
	mCurrentTurnSpeed = mMaxTurnSpeed * (mLastTurnX - turnEvent->GetTurn());
	mLastTurnX = turnEvent->GetTurn();

	float rotationAngle = CalculateTurnPosition(elapsedTime, mCurrentTurnSpeed);
	
	auto transformation = GetTransformation();
	glm::vec3 rotation = transformation->GetRotation();
	rotation.y = rotationAngle;
	transformation->SetRotation(rotation);

	glm::vec3 newVelocity = CalculateRunPosition(elapsedTime, rotationAngle, 0, mPhysicsComponent->GetVelocity(), mCurrentRunSpeed);
	mPhysicsComponent->SetVelocity(newVelocity);
}

void Player::TreatMoveEvent(float elapsedTime, std::shared_ptr<GameEvent> event)
{
	bool isForward = false;
	std::shared_ptr<CharacterControllerEvent> characterEvent;
	if (event->IsOfType<ForwardEvent>())
	{
		characterEvent = std::dynamic_pointer_cast<ForwardEvent>(event);
		isForward = true;
	}
	else if (event->IsOfType<BackwardEvent>())
	{
		characterEvent = std::static_pointer_cast<BackwardEvent>(event);
	}

	bool isPressed = characterEvent->IsPressed();

	isForward ? mActions[PlayerAction::Forward] = isPressed : mActions[PlayerAction::Backward] = isPressed;

	UpdateVelocity(isPressed, isForward);
}

void Player::TreatJumpEvent(float elapsedTime, std::shared_ptr<GameEvent> event)
{
	std::shared_ptr<const JumpEvent> jumpEvent = std::static_pointer_cast<const JumpEvent>(event);

	mActions[PlayerAction::Jump] = jumpEvent->IsPressed();
}

float Player::CalculateTurnPosition(float elapsedTime, float turnSpeed)
{
	if (turnSpeed != 0.0f)
	{
		mCurrentTurnAngle += turnSpeed * elapsedTime;
	}
	return mCurrentTurnAngle;
}

glm::vec3 Player::CalculateRunPosition(float elapsedTime, float rotY, float rotOffset, glm::vec3 velocity, float runSpeed)
{
	velocity.x = glm::sin(rotY + rotOffset) * runSpeed;
	velocity.z = glm::cos(rotY + rotOffset) * runSpeed;

	return velocity;
}

void Player::UpdateVelocity(bool isMoving, bool isForward)
{
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

void Player::PlayAnimation(const std::string& animationName)
{
	mAnimationComponent->PlayAnimation(animationName);
}

void Player::StopAnimations()
{
	mAnimationComponent->StopAnimation();
}

void Player::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);
	source->ReadParameter("run_speed", &mMaxRunSpeed);
	mRunSpeed = mMaxRunSpeed;
	source->ReadParameter("turn_speed", &mMaxTurnSpeed);
	source->ReadParameter("upwards_speed", &mMaxUpwardsSpeed);
}

/*
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
				TreatForwardEvent(event);
				TreatBackwardEvent(event);

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
				TreatJumpEvent(event);
				TreatTurnEvent(event);

				break;
			case FALLING:
				TreatTurnEvent(event);
				TreatBackwardEvent(event);
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


void Player::UpdateMoving(float elapsedTime)
{
	Transformation* transformation = GetTransformation();
	bool isOnGround = GetComponent<CollisionComponent>()->IsOnGround();

	if (isOnGround)
	{
		if (mHasJumped)
		{
			mState = JUMPING;
		}
		else if (!IsMoving())
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
	else if (IsFalling())
	{
		mState = FALLING;
	}
}


bool Player::IsFalling()
{
	auto transformation = GetTransformation();
	auto collisionComponent = GetComponent<CollisionComponent>();
	bool isOnGround = collisionComponent->IsOnGround();
	auto groundHeight = collisionComponent->GetGroundHeight();
	bool isFalling = !isOnGround && glm::abs(transformation->GetPosition().y - groundHeight) > MIN_HEIGHT_FALL;

	return isFalling;
}

bool Player::IsMoving()
{
	auto physicsComponent = GetComponent<PhysicsComponent>();
	return glm::length(physicsComponent->GetVelocity()) > 0;
}
*/