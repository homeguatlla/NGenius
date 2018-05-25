#include "stdafx.h"
#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane) :
mFov(fov),
mAspectRatio(aspectRatio),
mNearPlane(nearPlane),
mFarPlane(farPlane)
{
	SetName(name);
	mIsDirty = true;
	CreateProjectionMatrix();
}


PerspectiveCamera::~PerspectiveCamera()
{
}

AABB PerspectiveCamera::GetAABB() const
{
	glm::vec3 forward = glm::normalize(mTarget - mPosition);
	glm::vec3 nearCenter = mPosition + forward * mNearPlane;
	glm::vec3 farCenter = mPosition + forward * mFarPlane;
	
	float fovRadians = glm::tan(glm::radians(mFov) * 0.5f);
	float nearHeight = 2.0f * fovRadians * mNearPlane;
	float farHeight = 2.0f * fovRadians * mFarPlane;
	float nearWidth = nearHeight * mAspectRatio;
	float farWidth = farHeight * mAspectRatio;
	
	float farWidth2 = farWidth * 0.5f;
	float nearWidth2 = nearWidth * 0.5f;
	float farHeight2 = farHeight * 0.5f;
	float nearHeight2 = nearHeight * 0.5f;

	glm::vec3 right = glm::cross(mUp, forward);
	glm::vec3 up = glm::cross(right, forward);

	glm::vec3 points[8];

	points[0] = farCenter + up * farHeight2 - right * farWidth2; //far top left
	points[1]= farCenter + up * farHeight2 + right * farWidth2; //far top right
	points[2]= farCenter - up * farHeight2 - right * farWidth2;//far bottom left
	points[3]= farCenter - up * farHeight2 + right * farWidth2;//far bottom right

	points[4]= nearCenter + up * nearHeight2 - right * nearWidth2;//near top left
	points[5]= nearCenter + up * nearHeight2 + right * nearWidth2;//near top right
	points[6]= nearCenter - up * nearHeight2 - right * nearWidth2;//near bottom left
	points[7]= nearCenter - up * nearHeight2 + right * nearWidth2;//near bottom right

	glm::vec3 min(points[0]);
	glm::vec3 max(points[0]);

	for (int i = 1; i < 8; ++i)
	{
		min.x = glm::min(min.x, points[i].x);
		min.y = glm::min(min.y, points[i].y);
		min.z = glm::min(min.z, points[i].z);

		max.x = glm::max(max.x, points[i].x);
		max.y = glm::max(max.y, points[i].y);
		max.z = glm::max(max.z, points[i].z);
	}

	AABB aabb(min, max);

	return aabb;
}

void PerspectiveCamera::FillWithProjectedVolume(std::vector<glm::vec2>& points, float fovDilatation) const
{
	glm::vec3 forward = glm::normalize(mTarget - mPosition);
	glm::vec3 nearCenter = mPosition + forward * mNearPlane;
	glm::vec3 farCenter = mPosition + forward * mFarPlane;

	float fovRadians = glm::tan(glm::radians(mFov + fovDilatation) * 0.5f);
	float nearHeight = 2.0f * fovRadians * mNearPlane;
	float farHeight = 2.0f * fovRadians * mFarPlane;
	float nearWidth = nearHeight * mAspectRatio;
	float farWidth = farHeight * mAspectRatio;

	float farWidth2 = farWidth * 0.5f;
	float nearWidth2 = nearWidth * 0.5f;
	float farHeight2 = farHeight * 0.5f;
	float nearHeight2 = nearHeight * 0.5f;

	glm::vec3 right = glm::cross(mUp, forward);
	glm::vec3 up = glm::cross(right, forward);

	glm::vec3 point = farCenter - up * farHeight2 - right * farWidth2; //far bottom left
	points.push_back(glm::vec2(point.x, point.z));
	
	point = farCenter - up * farHeight2 + right * farWidth2;//far bottom right
	points.push_back(glm::vec2(point.x, point.z));

	point = nearCenter - up * nearHeight2 - right * nearWidth2;//near bottom left
	points.push_back(glm::vec2(point.x, point.z));

	point = nearCenter - up * nearHeight2 + right * nearWidth2;//near bottom right
	points.push_back(glm::vec2(point.x, point.z));
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