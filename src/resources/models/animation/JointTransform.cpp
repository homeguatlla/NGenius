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
	/*
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mLocalTransform, scale, rotation, translation, skew, perspective);
	
	mPosition = translation;
	mRotation = glm::quat(glm::vec3(rotation[0], rotation[1], rotation[2]));
	mRotation = glm::normalize(mRotation);*/

	mPosition.x = mLocalTransform[0][3];
	mPosition.y = mLocalTransform[1][3];
	mPosition.z = mLocalTransform[2][3];
	
	mRotation = FromMatrix(mLocalTransform);
}

glm::quat JointTransform::FromMatrix(const glm::mat4x4& matrix) 
{
	float w, x, y, z;
	float diagonal = matrix[0][0] + matrix[1][1] + matrix[2][2];
	if (diagonal > 0) {
		float w4 = (float)(sqrt(diagonal + 1.f) * 2.f);
		w = w4 / 4.f;
		x = (-matrix[2][1] + matrix[1][2]) / w4;
		y = (-matrix[0][2] + matrix[2][0]) / w4;
		z = (-matrix[1][0] + matrix[0][1]) / w4;
	}
	else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2])) {
		float x4 = (float)(sqrt(1.f + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2.f);
		w = (-matrix[2][1] + matrix[1][2]) / x4;
		x = x4 / 4.f;
		y = (matrix[0][1] + matrix[1][0]) / x4;
		z = (matrix[0][2] + matrix[2][0]) / x4;
	}
	else if (matrix[1][1] > matrix[2][2]) {
		float y4 = (float)(sqrt(1.f + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2.f);
		w = (-matrix[0][2] + matrix[2][0]) / y4;
		x = (matrix[0][1] + matrix[1][0]) / y4;
		y = y4 / 4.f;
		z = (matrix[1][2] + matrix[2][1]) / y4;
	}
	else {
		float z4 = (float)(sqrt(1.f + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2.f);
		w = (-matrix[1][0] + matrix[0][1]) / z4;
		x = (matrix[0][2] + matrix[2][0]) / z4;
		y = (matrix[1][2] + matrix[2][1]) / z4;
		z = z4 / 4.f;
	}
	return glm::quat(w, x, y, z);
}
/*
glm::quat JointTransform::FromMatrix(const glm::mat4x4& matrix)
{
	float w, x, y, z;
	float diagonal = matrix[0][0] + matrix[1][1] + matrix[2][2];
	if (diagonal > 0) {
		float w4 = (float)(sqrt(diagonal + 1.f) * 2.f);
		w = w4 / 4.f;
		x = (matrix[2][1] - matrix[1][2]) / w4;
		y = (matrix[0][2] - matrix[2][0]) / w4;
		z = (matrix[1][0] - matrix[0][1]) / w4;
	}
	else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2])) {
		float x4 = (float)(sqrt(1.f + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2.f);
		w = (matrix[2][1] - matrix[1][2]) / x4;
		x = x4 / 4.f;
		y = (matrix[0][1] + matrix[1][0]) / x4;
		z = (matrix[0][2] + matrix[2][0]) / x4;
	}
	else if (matrix[1][1] > matrix[2][2]) {
		float y4 = (float)(sqrt(1.f + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2.f);
		w = (matrix[0][2] - matrix[2][0]) / y4;
		x = (matrix[0][1] + matrix[1][0]) / y4;
		y = y4 / 4.f;
		z = (matrix[1][2] + matrix[2][1]) / y4;
	}
	else {
		float z4 = (float)(sqrt(1.f + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2.f);
		w = (matrix[1][0] - matrix[0][1]) / z4;
		x = (matrix[0][2] + matrix[2][0]) / z4;
		y = (matrix[1][2] + matrix[2][1]) / z4;
		z = z4 / 4.f;
	}
	return glm::quat(x, y, z, w);
}*/
glm::mat4x4& JointTransform::ToRotationMatrix() 
{
	float xy = mRotation.x * mRotation.y;
	float xz = mRotation.x * mRotation.z;
	float xw = mRotation.x * mRotation.w;
	float yz = mRotation.y * mRotation.z;
	float yw = mRotation.y * mRotation.w;
	float zw = mRotation.z * mRotation.w;
	float xSquared = mRotation.x * mRotation.x;
	float ySquared = mRotation.y * mRotation.y;
	float zSquared = mRotation.z * mRotation.z;
	
	glm::mat4x4 matrix;

	matrix[0][0] = 1.0f - 2.0f * (ySquared + zSquared);
	matrix[1][0] = 2.0f * (xy - zw);
	matrix[2][0] = 2.0f * (xz + yw);
	matrix[3][0] = 0.0f;
	matrix[0][1] = 2.0f * (xy + zw);
	matrix[1][1] = 1.0f - 2.0f * (xSquared + zSquared);
	matrix[2][1] = 2.0f * (yz - xw);
	matrix[3][1] = 0.0f;
	matrix[0][2] = 2.0f * (xz - yw);
	matrix[1][2] = 2.0f * (yz + xw);
	matrix[2][2] = 1.0f - 2.0f * (xSquared + ySquared);
	matrix[3][2] = 0.0f;
	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
	matrix[3][3] = 1.0f;
	return matrix;
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
	glm::quat rotation = glm::slerp(frameA->GetRotation(), frameB->GetRotation(), progression);

	return JointTransform(position, rotation);
}

void JointTransform::CalculateLocalTransform()
{
	mLocalTransform = glm::mat4x4(1.0f);
	//mLocalTransform = glm::translate(mLocalTransform, mPosition);
	mLocalTransform[0][3] = mPosition.x;
	mLocalTransform[1][3] = mPosition.y;
	mLocalTransform[2][3] = mPosition.z;

	glm::mat4x4 temp = glm::mat4_cast(mRotation);
	glm::mat4x4 temp2 = ToRotationMatrix();
	glm::mat4x4 temp3 = glm::toMat4(mRotation);
	temp = temp * mLocalTransform;
	temp2 = temp2 * mLocalTransform;
	temp3 = temp3 * mLocalTransform;

	mLocalTransform = temp2;
}