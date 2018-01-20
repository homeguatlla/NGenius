#pragma once
#include "ICamera.h"

class PerspectiveCamera : public ICamera
{
public:
	explicit PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);
	~PerspectiveCamera();

private:
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mFov;
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;
};

