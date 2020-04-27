#pragma once
#include "src/resources/BaseGameEntity.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"
#include "src/utils/fsm/StatesMachine.h"
#include <memory>

class IRenderer;
class Transformation;
class InputComponent;
class CollisionComponent;
class PhysicsComponent;
class CharacterComponent;
class AnimationComponent;

class Player : public BaseGameEntity<Player>
{
public:
	Player();
	explicit Player(Transformation* transformation, IRenderer* renderer, InputComponent* playerInputComponent, 
					CharacterComponent* characterComponent, PhysicsComponent* physicsComponent, 
					CollisionComponent* collisionComponent, float runSpeed, float turnSpeed, float upwardsSpeed);
	virtual ~Player() = default;

	std::shared_ptr<IGameEntity> DoClone() const override { return nullptr; }
	void Update(float elapsedTime) override;

	void DoInit(GameScene* scene, RenderSystem* renderSystem) override;

	void PlayAnimation(const std::string& animationName);
	void StopAnimations();

	void TreatTurnEvent(float elapsedTime, std::shared_ptr<GameEvent> event);
	void TreatMoveEvent(float elapsedTime, std::shared_ptr<GameEvent> event);

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	
	static std::string GetClassName() { return std::string("player"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	void CreateStatesMachine();
	float CalculateTurnPosition(float elapsedTime, float turnSpeed);
	glm::vec3 CalculateRunPosition(float elapsedTime, float rotY, float rotOffset, glm::vec3 velocity, float runSpeed);

	void UpdateVelocity(bool isMoving, bool isForward);

	/*
	void UpdateGameEvents();
	void UpdateIdle(float elapsedTime);
	void UpdateMoving(float elapsedTime);
	void UpdateJumping(float elapsedTime);
	void UpdateFalling(float elapsedTime);

	
	void UpdateSpeed(bool isAccelerating);
	void UpdateLateralAngle(bool isMoving, bool isLeft);

	void TreatJumpEvent(std::shared_ptr<const GameEvent> event);
	void TreatForwardEvent(std::shared_ptr<const GameEvent> event);
	void TreatBackwardEvent(std::shared_ptr<const GameEvent> event);

	glm::vec3 CalculateJumpPosition(float elapsedTime, glm::vec3 velocity, float upwardsSpeed);
	*/

private:
	std::unique_ptr<core::utils::FSM::StatesMachine<PlayerState, PlayerContext>> mStatesMachine;
	std::shared_ptr<PlayerContext> mPlayerContext;

	float mMaxRunSpeed;
	float mMaxTurnSpeed;
	float mMaxUpwardsSpeed;

	float mCurrentTurnSpeed;
	float mLastTurnX;

	float mCurrentTurnAngle;

	float mRunSpeed;
	float mCurrentRunSpeed;

	std::shared_ptr<AnimationComponent> mAnimationComponent;
	std::shared_ptr<PhysicsComponent> mPhysicsComponent;
	std::shared_ptr<CharacterComponent> mCharacterComponent;
	std::shared_ptr<CollisionComponent> mCollisionComponent;

	/*
	float mCurrentUpwardsSpeed;
	float mLastTurnX;
	float mCurrentTurnAngle;
	float mOriginalTurnAngle;
	float mRotOffsetForLateralMoving;

	bool mHasMoved;
	bool mHasJumped;
	bool mIsShiftPressed;*/
};

