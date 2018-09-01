#include "stdafx.h"
#include "JointTransform.h"
#include "glm/gtc/matrix_transform.hpp"


JointTransform::JointTransform(glm::vec3& position, glm::quat& rotation) :
	mPosition(position),
	mRotation(rotation)
{
	CalculateLocalTransform();
}

JointTransform::~JointTransform()
{
}

const glm::mat4x4& JointTransform::GetLocalTransform() const
{
	return mLocalTransform;
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
	glm::quat rotation = glm::mix(frameA->GetRotation(), frameB->GetRotation(), progression);

	return JointTransform(position, rotation);
}

void JointTransform::CalculateLocalTransform()
{
	mLocalTransform = glm::mat4x4(1.0f);
	glm::translate(mLocalTransform, mPosition);
	mLocalTransform = mLocalTransform * glm::mat4_cast(mRotation);
}