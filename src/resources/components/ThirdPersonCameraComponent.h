#pragma once
#include "InputComponent.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>

class IGameEntity;
class ICamera;
class PerspectiveCamera;

class ThirdPersonCameraComponent : public IComponent
{
	ICamera* mCamera;
	std::shared_ptr<IGameEntity> mTarget;
	glm::vec3 mTargetOffset;
	float mDistanceFromTarget;
	float mPitch;
	float mPitchSpeed;
	float mCurrentPitch;
	float mLastPitch;
	float mAngleAroundTarget;
	float mZoomSpeed;
	bool mIsCameraFollowingTarget;
	std::string mCameraName;
	std::string mTargetName;

public:
	explicit ThirdPersonCameraComponent(PerspectiveCamera* camera, std::shared_ptr<IGameEntity> target, const glm::vec3& targetOffset, float distanceFromTarget, float pitch, float pitchSpeed, float zoomSpeed);
	~ThirdPersonCameraComponent();

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	void UpdateInternal(float elapsedTime) override;
	const std::shared_ptr<IGameEntity> GetTarget() const;
	glm::vec3 GetCameraPosition() const;
	float GetCameraPitch() const;

	static std::string GetClassName() { return std::string("ThirdPersonCameraComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	ThirdPersonCameraComponent();
	void UpdateGameEvents(float elapsedTime);
	void UpdateZoom(float scroll, float elapsedTime);
	void UpdatePitch(float pitch, float elapsedTime);
	float CalculateHorizontalDistance() const;
	float CalculateVerticalDistance() const;
	glm::vec3 CalculateCameraPosition(float horizontalDistance, float verticalDistance) const;

	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	ThirdPersonCameraComponent* DoClone() const;
};

