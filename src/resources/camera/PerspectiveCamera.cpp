#include "stdafx.h"
#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane) : 
mFov(fov), 
mAspectRatio(aspectRatio), 
mNearPlane(nearPlane), 
mFarPlane(farPlane)
{
	mIsDirty = true;
	CreateProjectionMatrix();
}


PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::CreateViewMatrix()
{
	mViewMatrix = glm::lookAt(
		GetPosition(), // the position of your camera, in world space
		mTarget,   // where you want to look at, in world space
		mUp        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
		);
	mIsDirty = false;
}

void PerspectiveCamera::CreateProjectionMatrix()
{
	mProjectionMatrix = glm::perspective(
		mFov,					// The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		mAspectRatio,			// Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		mNearPlane,				// Near clipping plane. Keep as big as possible, or you'll get precision issues.
		mFarPlane				// Far clipping plane. Keep as little as possible.
		);
}