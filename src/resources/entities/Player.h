#pragma once
#include "../GameEntity.h"

class IRenderer;
class Transformation;
class PlayerInputComponent;
class CollisionComponent;
class PhysicsComponent;

class Player : public GameEntity
{
	enum State
	{
		IDLE = 0,
		MOVING = 1,
		FLYING = 2,
		JUMPING = 3,
		FALLING = 4
	};

	State mState;

public:
	explicit Player(Transformation* transformation, IRenderer* renderer, PlayerInputComponent* playerInputComponent, PhysicsComponent* physicsComponent, CollisionComponent* collisionComponent);
	virtual ~Player();

	Player* DoClone() const override { return nullptr; }
	void Update(float elapsedTime) override;

private:
	void UpdateIdle(float elapsedTime);
	void UpdateMoving(float elapsedTime);
	void UpdateFlying(float elapsedTime);
	void UpdateJumping(float elapsedTime);
	void UpdateFalling(float elapsedTime);

	glm::vec3 CalculateRunPosition(float elapsedTime, Transformation* transformation, glm::vec3 velocity, float runSpeed);
	void CalculateTurnPosition(float elapsedTime, Transformation* transformation, float turnSpeed);
	glm::vec3 CalculateJumpPosition(float elapsedTime, Transformation* transformation, glm::vec3 velocity, float upwardsSpeed);
};

