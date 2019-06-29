#pragma once
#include "ICamera.h"
class OrthogonalCamera : public ICamera
{
public:
	explicit OrthogonalCamera(std::string name, float screenWidth, float screenHeight, float nearPlane, float farPlane);
	~OrthogonalCamera();
	AABB GetAABB() const override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	static std::string GetClassName() { return std::string("OrthogonalCamera"); }
	static ICamera* Create(std::string name, float screenWidth, float screenHeight, float nearPlane, float farPlane)
	{
		return new OrthogonalCamera(name, screenWidth, screenHeight, nearPlane, farPlane);
	}

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

