#pragma once
#include "InputComponent.h"
#include <glm/glm.hpp>

class GameEntity;
class PerspectiveCamera;

class ThirdPersonCameraComponent : public IComponent
{
	PerspectiveCamera* mCamera;
	GameEntity* mTarget;
	glm::vec3 mTargetOffset;
	float mDistanceFromTarget;
	float mPitch;
	float mPitchSpeed;
	float mCurrentPitch;
	float mLastPitch;
	float mAngleAroundTarget;
	const float mZoomSpeed;
	bool mIsCameraFollowingTarget;
	
public:
	explicit ThirdPersonCameraComponent(PerspectiveCamera* camera, GameEntity* target, const glm::vec3& targetOffset, float distanceFromTarget, float pitch, float pitchSpeed, float zoomSpeed);
	~ThirdPersonCameraComponent();

	void UpdateInternal(float elapsedTime) override;
	const GameEntity* GetTarget() const;
	glm::vec3 GetCameraPosition() const;
	float GetCameraPitch() const;

	static std::string GetClassName() { return std::string("ThirdPersonCameraComponent"); }
	static IComponent* Create();

private:
	ThirdPersonCameraComponent();
	void UpdateGameEvents(float elapsedTime);
	void UpdateZoom(float scroll, float elapsedTime);
	void UpdatePitch(float pitch, float elapsedTime);
	float CalculateHorizontalDistance() const;
	float CalculateVerticalDistance() const;
	glm::vec3 CalculateCameraPosition(float horizontalDistance, float verticalDistance) const;

	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	ThirdPersonCameraComponent* DoClone() const;
};

