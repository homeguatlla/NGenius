#include "stdafx.h"
#include "Transformation.h"
#include "../utils/serializer/XMLSerializer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <assert.h>

Transformation::Transformation() :
	mPosition(glm::vec3(0.0f)),
	mRotation(glm::vec3(0.0f)),
	mScale(glm::vec3(1.0f)),
	mIsDirty(true)
{

}

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
		if (mRotation.z != 0.0f)
		{
			mModelMatrix = glm::rotate(mModelMatrix, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (mRotation.y != 0.0f)
		{
			mModelMatrix = glm::rotate(mModelMatrix, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (mRotation.x != 0.0f)
		{
			mModelMatrix = glm::rotate(mModelMatrix, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		mModelMatrix = glm::scale(mModelMatrix, mScale);


		/*glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPosition);
		glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);

		mModelMatrix = translation * rotationZ * rotationY * rotationX * scale;

		assert(mModelMatrix == finalmatrix);*/
		mIsDirty = false;
	}
	return mModelMatrix;
}

void Transformation::ReadFrom(core::utils::IDeserializer* source)
{
}

void Transformation::WriteTo(core::utils::ISerializer* destination)
{
	destination->BeginAttribute(std::string("transformation"));
	
	destination->WriteParameter(std::string("position"), mPosition);
	destination->WriteParameter(std::string("rotation"), mRotation);
	destination->WriteParameter(std::string("scale"), mScale);

	destination->EndAttribute();
}
