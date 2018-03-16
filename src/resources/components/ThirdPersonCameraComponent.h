#pragma once
#include "InputComponent.h"
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
	const float mZoomSpeed;
	float mCurrentZoomSpeed;
public:
	explicit ThirdPersonCameraComponent(PerspectiveCamera* camera, GameEntity* target, float distanceFromTarget, float pitch, float zoomSpeed);
	~ThirdPersonCameraComponent();

	ThirdPersonCameraComponent* DoClone() const;

	void Update(float elapsedTime) override;
	const GameEntity* GetTarget() const;
	glm::vec3 GetCameraPosition() const;

private:
	void UpdateGameEvents();
	void UpdateZoomSpeed(float scroll);
	float CalculateHorizontalDistance() const;
	float CalculateVerticalDistance() const;
	glm::vec3 CalculateCameraPosition(float horizontalDistance, float verticalDistance) const;
};

