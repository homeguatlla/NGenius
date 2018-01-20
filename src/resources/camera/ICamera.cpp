#include "stdafx.h"
#include "ICamera.h"
#include <glm/gtc/matrix_transform.hpp>


ICamera::~ICamera()
{
}

const glm::mat4& ICamera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

const glm::mat4& ICamera::GetViewMatrix()
{
	if (mIsDirty)
	{
		CreateViewMatrix();
	}
	return mViewMatrix;
}


void ICamera::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	mIsDirty = true;
}

void ICamera::SetTarget(const glm::vec3& target)
{
	mTarget = target;
	mIsDirty = true;
}

void ICamera::SetUp(const glm::vec3& up)
{
	mUp = up;
	mIsDirty = true;
}

glm::vec3 ICamera::GetPosition() const
{
	return mPosition;
}

glm::vec3 ICamera::GetTarget() const
{
	return mTarget;
}

glm::vec3 ICamera::GetUp() const
{
	return mUp;
}

void ICamera::Move(float speed)
{
	glm::vec3 position = GetPosition();
	position += glm::normalize(mTarget - position) * speed;
	mIsDirty = true;

	SetPosition(position);
}

void ICamera::Rotate(float angle, const glm::vec3& axis)
{
	glm::vec3 position = GetPosition();
	glm::vec3 direction = mTarget - position;
	direction = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(direction, 1.0));
	mTarget = position + direction;
	mIsDirty = true;

	SetPosition(position);
}
