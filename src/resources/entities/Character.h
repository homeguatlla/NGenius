#pragma once
#include "../GameEntity.h"

class IRenderer;
class Transformation;
class InputComponent;
class CollisionComponent;
class PhysicsComponent;
class CharacterComponent;

class Character : public GameEntity
{
	enum State
	{
		IDLE = 0,
		MOVING,
		JUMPING,
		FALLING
	};

	State mState;
	const float mRunSpeed;
	const float mTurnSpeed;
	const float mUpwardsSpeed;
	float mCurrentRunSpeed;
	float mCurrentTurnSpeed;
	float mCurrentUpwardsSpeed;
	float mLastTurnX;
	float mCurrentTurnAngle;
	float mOriginalTurnAngle;

	bool mHasMoved;
	bool mHasJumped;

public:
	explicit Character(Transformation* transformation, IRenderer* renderer, InputComponent* playerInputComponent, 
					CharacterComponent* characterComponent, PhysicsComponent* physicsComponent, 
					CollisionComponent* collisionComponent, float runSpeed, float turnSpeed, float upwardsSpeed);
	virtual ~Character();

	Character* DoClone() const override { return nullptr; }
	void Update(float elapsedTime) override;

private:
	void UpdateGameEvents();
	void UpdateIdle(float elapsedTime);
	void UpdateMoving(float elapsedTime);
	void UpdateJumping(float elapsedTime);
	void UpdateFalling(float elapsedTime);

	void UpdateAnimations();

	glm::vec3 CalculateRunPosition(float elapsedTime, float rotY, glm::vec3 velocity, float runSpeed);
	float CalculateTurnPosition(float elapsedTime, float turnSpeed);
	glm::vec3 CalculateJumpPosition(float elapsedTime, glm::vec3 velocity, float upwardsSpeed);
};

