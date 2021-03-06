#pragma once
#include "src/resources/BaseGameEntity.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"
#include "src/utils/fsm/StatesMachine.h"
#include <memory>
#include <map>

class IRenderer;
class Transformation;
class InputComponent;
class CollisionComponent;
class PhysicsComponent;
class CharacterComponent;
class AnimationComponent;

enum class PlayerAction
{
	Forward = 0,
	Backward = 1, 
	Jump = 2,
	Run = 3
};

class Player : public BaseGameEntity<Player>
{
public:
	Player();
	explicit Player(Transformation* transformation, IRenderer* renderer, InputComponent* playerInputComponent, 
					CharacterComponent* characterComponent, PhysicsComponent* physicsComponent, 
					CollisionComponent* collisionComponent, float walkSpeed, float runSpeed, float turnSpeed, float upwardsSpeed);
	virtual ~Player() = default;

	std::shared_ptr<IGameEntity> DoClone() const override { return nullptr; }
	void Update(float elapsedTime) override;

	void DoInit(GameScene* scene, RenderSystem* renderSystem) override;

	bool IsActionActive(PlayerAction action) { return mActions[action]; }

	void PerformMovement(float elapsedTime);
	void PerformJump(float elapsedTime);
	void EnableRun(bool enable);

	void PlayAnimation(const std::string& animationName);
	void StopAnimations();

	// Heredado v�a ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	
	static std::string GetClassName() { return std::string("player"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	void CreateStatesMachine();
	void UpdateEvents(float deltaTime);

	float CalculateTurnPosition(float elapsedTime, float turnSpeed);
	glm::vec3 CalculateRunPosition(float elapsedTime, float rotY, float rotOffset, glm::vec3 velocity, float runSpeed);

	void TreatTurnEvent(float elapsedTime, std::shared_ptr<GameEvent> event);
	void TreatMoveEvent(float elapsedTime, std::shared_ptr<GameEvent> event);
	void TreatJumpEvent(float elapsedTime, std::shared_ptr<GameEvent> event);
	void TreatRunEvent(float elapsedTime, std::shared_ptr<GameEvent> event);

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

	float mMaxWalkSpeed;
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

	std::map<PlayerAction, bool> mActions;

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

