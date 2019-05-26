#pragma once
#include "ICamera.h"

class PerspectiveCamera : public ICamera
{
public:
	PerspectiveCamera() = default;
	explicit PerspectiveCamera(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane);
	~PerspectiveCamera();
	AABB GetAABB() const override;

	ICamera* CreateCamera() override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

private:
	void Initialize();
	void CalculateFrustum() override;
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mFov;
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;
};

