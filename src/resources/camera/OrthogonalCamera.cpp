#include "stdafx.h"
#include "OrthogonalCamera.h"
#include <glm/gtc/matrix_transform.hpp>

static const int FAR_PLANE = 1000;
static const int NEAR_PLANE = 0;
static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

OrthogonalCamera::OrthogonalCamera(std::string name, float screenWidth, float screenHeight, float nearPlane, float farPlane) :
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mNearPlane(nearPlane),
mFarPlane(farPlane)
{
	SetName(name);
	mIsDirty = true;
	CreateProjectionMatrix();
}

OrthogonalCamera::~OrthogonalCamera()
{
}

AABB OrthogonalCamera::GetAABB() const
{
	AABB aabb;
	return aabb;
}

void OrthogonalCamera::CreateViewMatrix()
{
	mViewMatrix = glm::lookAt(
		GetPosition(), // the position of your camera, in world space
		mTarget,   // where you want to look at, in world space
		mUp        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
		);
	mIsDirty = false;
}

void OrthogonalCamera::CreateProjectionMatrix()
{
	mProjectionMatrix = glm::ortho(-mScreenWidth * 0.5f, mScreenWidth * 0.5f, -mScreenHeight * 0.5f, mScreenHeight * 0.5f, mNearPlane, mFarPlane);
}