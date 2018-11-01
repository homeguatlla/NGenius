#include "stdafx.h"
#include "JointTransform.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>

JointTransform::JointTransform(glm::vec3& position, glm::quat& rotation) :
	mPosition(position),
	mRotation(rotation)
{
	mRotation = glm::normalize(mRotation);
	CalculateLocalTransform();
}

JointTransform::JointTransform(const glm::mat4x4& localTransformation) :
	mLocalTransform(localTransformation)
{
	CalculatePositionRotation();
}

JointTransform::~JointTransform()
{
}

const glm::mat4x4& JointTransform::GetLocalTransform() const
{
	return mLocalTransform;
}

void JointTransform::SetLocalTransform(const glm::mat4x4& matrix)
{
	mLocalTransform = matrix;
	CalculatePositionRotation();
}

glm::vec3 JointTransform::GetPosition() const
{
	return mPosition;
}

glm::quat JointTransform::GetRotation() const
{
	return mRotation;
}

JointTransform JointTransform::Interpolate(JointTransform* frameA, JointTransform* frameB, float progression)
{
	assert(progression >= 0.0f && progression <= 1.0f);

	glm::vec3 position = glm::mix(frameA->GetPosition(), frameB->GetPosition(), progression);
	glm::quat rotation = glm::slerp(frameA->GetRotation(), frameB->GetRotation(), progression);

	return JointTransform(position, rotation);
}

void JointTransform::CalculatePositionRotation()
{
	mPosition = glm::vec3(mLocalTransform[3]);
	mRotation = glm::toQuat(mLocalTransform);
}

void JointTransform::CalculateLocalTransform()
{
	mLocalTransform = glm::mat4x4(1.0f);
	mLocalTransform = glm::translate(mLocalTransform, mPosition) * glm::toMat4(mRotation);
}