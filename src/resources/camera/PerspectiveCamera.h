#pragma once
#include "ICamera.h"
#include "../Memory.h"

class PerspectiveCamera : public ICamera
{
public:
	explicit PerspectiveCamera(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane);
	~PerspectiveCamera();
	AABB GetAABB() const override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	static std::string GetClassName() { return std::string("PerspectiveCamera"); }
	static ICamera* Create(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		return DBG_NEW PerspectiveCamera(name, fov, aspectRatio, nearPlane, farPlane);
	}

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

