#pragma once
#include "ICamera.h"

class PerspectiveCamera : public ICamera
{
public:
	explicit PerspectiveCamera(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane);
	~PerspectiveCamera();
	AABB GetAABB() const override;

private:
	void CalculateFrustum() override;
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mFov;
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;
};

