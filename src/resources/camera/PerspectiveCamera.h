#pragma once
#include "ICamera.h"

class PerspectiveCamera : public ICamera
{
public:
	explicit PerspectiveCamera(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane);
	~PerspectiveCamera();
	AABB GetAABB() const override;
	void FillWithProjectedVolume(std::vector<glm::vec2>& points, float fovDilatation) const;

private:
	void CreateViewMatrix() override;
	void CreateProjectionMatrix() override;

	float mFov;
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;
};

