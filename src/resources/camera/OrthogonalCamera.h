#pragma once
#include "ICamera.h"
class OrthogonalCamera : public ICamera
{
public:
	OrthogonalCamera(std::string name, float screenWidth, float screenHeight, float nearPlane, float farPlane);
	~OrthogonalCamera();
	AABB GetAABB() const override;

private:
	void CalculateFrustum() override;
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mScreenWidth;
	float mScreenHeight;
	float mNearPlane;
	float mFarPlane;
};

