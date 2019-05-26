#pragma once
#include "ICamera.h"
class OrthogonalCamera : public ICamera
{
public:
	OrthogonalCamera() = default;
	explicit OrthogonalCamera(std::string name, float screenWidth, float screenHeight, float nearPlane, float farPlane);
	~OrthogonalCamera();
	AABB GetAABB() const override;
	ICamera* CreateCamera() override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

private:
	void Initialize();
	void CalculateFrustum() override;
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mScreenWidth;
	float mScreenHeight;
	float mNearPlane;
	float mFarPlane;
};

