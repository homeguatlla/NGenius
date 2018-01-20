#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class GameEntity;
class PerspectiveCamera;

class ThirdPersonCameraComponent : public IComponent
{
	PerspectiveCamera* mCamera;
	GameEntity* mTarget;
	float mDistanceFromTarget;
	float mPitch;
	float mAngleAroundTarget;

public:
	explicit ThirdPersonCameraComponent(PerspectiveCamera* camera, GameEntity* target, float distanceFromTarget, float pitch);
	~ThirdPersonCameraComponent();

	ThirdPersonCameraComponent* DoClone() const;

	void Update(float elapsedTime) override;
	const GameEntity* GetTarget() const;
	glm::vec3 GetCameraPosition() const;

private:
	float CalculateHorizontalDistance() const;
	float CalculateVerticalDistance() const;
	glm::vec3 CalculateCameraPosition(float horizontalDistance, float verticalDistance) const;
};

