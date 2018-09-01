#include "stdafx.h"
#include "AnimateModel.h"
#include "Joint.h"
#include "../Model.h"

AnimateModel::AnimateModel(const Model* model, Joint* rootJoint) :
	mModel(model),
	mRootJoint(rootJoint)
{
	mRootJoint->CalculateInverseBindTransform(glm::mat4(1.0f));
}

AnimateModel::~AnimateModel()
{
}

const Model* AnimateModel::GetModel() const
{
	return mModel;
}

Joint* AnimateModel::GetRootJoint()
{
	return mRootJoint;
}

//void SetAnimation(const Animation* animation)

void AnimateModel::FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const
{
	mRootJoint->FillWithJointTransforms(jointMatrices);
}

void AnimateModel::Update()
{
	//mAnimator.Update();
}

/*
void AnimateModel::CreateAnimationWeightsVBO(VertexBuffersManager& vertexBufferManager, int location)
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