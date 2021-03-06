#include "stdafx.h"
#include "Joint.h"


Joint::Joint(int index, std::string& name, glm::mat4& localBindTransformation) :
	mIndex(index),
	mName(name),
	mLocalBindTransformation(localBindTransformation)
{
}

Joint::~Joint()
{
}

const std::string& Joint::GetName() const
{
	return mName;
}
const std::vector<Joint*>& Joint::GetJoints() const
{
	return mChildren;
}

void Joint::AddChild(Joint* joint)
{
	mChildren.push_back(joint);
}

const glm::mat4& Joint::GetAnimatedTransform() const
{
	return mLocalBindTransformation;
}

void Joint::SetAnimatedTransform(const glm::mat4& animatedTransform)
{
	mLocalBindTransformation = animatedTransform;
}

const glm::mat4& Joint::GetInverseBindTransform() const
{
	return mInverseBindTransformation;
}

void Joint::FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const
{
	//TODO esto creo que si se quita por indice y se a�ade directamente con un push_back funcionar�a aunque no estoy muy seguro 
	//si para todas las animaciones
	jointMatrices[mIndex] = GetAnimatedTransform();

	for (Joint* child : mChildren)
	{
		child->FillWithJointTransforms(jointMatrices);
	}
}

void Joint::CalculateInverseBindTransform(const glm::mat4& parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform * mLocalBindTransformation;
	mInverseBindTransformation = glm::inverse(bindTransform);
	for (Joint* child : mChildren)
	{
		child->CalculateInverseBindTransform(bindTransform);
	}
}