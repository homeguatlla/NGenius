#pragma once
#include "ICamera.h"
class OrthogonalCamera : public ICamera
{
public:
	OrthogonalCamera(float screenWidth, float screenHeight, float nearPlane, float farPlane);
	~OrthogonalCamera();

private:
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mScreenWidth;
	float mScreenHeight;
	float mNearPlane;
	float mFarPlane;
};

