#include "stdafx.h"
#include "Transformation.h"
#include <glm/gtc/matrix_transform.hpp>

Transformation::Transformation(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
mPosition(position),
mRotation(rotation),
mScale(scale),
mIsDirty(true)
{
}


Transformation::~Transformation()
{
}

void Transformation::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	mIsDirty = true;
}

glm::vec3 Transformation::GetPosition() const
{
	return mPosition;
}

void Transformation::SetRotation(const glm::vec3& rotation)
{
	mRotation = rotation;
	mIsDirty = true;
}

glm::vec3 Transformation::GetRotation() const
{
	return mRotation;
}

void Transformation::SetScale(const glm::vec3& scale)
{
	mScale = scale;
	mIsDirty = true;
}

glm::vec3 Transformation::GetScale() const
{
	return mScale;
}

const glm::mat4& Transformation::GetModelMatrix()
{
	if (mIsDirty)
	{
		mModelMatrix = glm::translate(glm::mat4(1.0f), mPosition);
		mModelMatrix = glm::rotate(mModelMatrix, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		mModelMatrix = glm::rotate(mModelMatrix, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mModelMatrix = glm::rotate(mModelMatrix, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		mModelMatrix = glm::scale(mModelMatrix, mScale);
		mIsDirty = false;
	}
	return mModelMatrix;
}
