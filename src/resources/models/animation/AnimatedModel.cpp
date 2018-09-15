#include "stdafx.h"
#include "AnimatedModel.h"
#include "Joint.h"
#include "../Model.h"

AnimatedModel::AnimatedModel(const std::string& name, Mesh* mesh, Joint* rootJoint) :
	Model(mesh),
	mName(name),
	mRootJoint(rootJoint),
	mAnimator(this)
{
	mRootJoint->CalculateInverseBindTransform(glm::mat4(1.0f));
}

AnimatedModel::~AnimatedModel()
{
}

Joint* AnimatedModel::GetRootJoint()
{
	return mRootJoint;
}

std::string AnimatedModel::GetName() const
{
	return mName;
}

bool AnimatedModel::IsAnimatedModel() const
{
	return true;
}

void AnimatedModel::PlayAnimation(Animation* animation)
{
	mAnimator.PlayAnimation(animation);
}

void AnimatedModel::FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const
{
	mRootJoint->FillWithJointTransforms(jointMatrices);
}

void AnimatedModel::Update(float elapsedTime)
{
	mAnimator.Update(elapsedTime);
}

/*
void AnimatedModel::CreateAnimationWeightsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	//5th animation weights
	long numWeights = mJointCount;
	if (numWeights > 0)
	{
		std::string name("animation_weights_");
		name.append(std::to_string(GetID()));

		unsigned int weightsVBO = vertexBufferManager.CreateVBO(name);
		glGenBuffers(1, &weightsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, weightsVBO);
		glBufferData(GL_ARRAY_BUFFER, numWeights * sizeof(glm::vec3), &mMesh->GetNormals()[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}*/