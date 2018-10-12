#include "stdafx.h"
#include "AnimatedModel.h"
#include "Joint.h"
#include "../Model.h"
#include "../Mesh.h"
#include "../../materials/IMaterial.h"
#include "../../shaders/IShaderProgram.h"

#include <GL/glew.h>

AnimatedModel::AnimatedModel(const std::string& name, Mesh* mesh, Joint* rootJoint) :
	Model(mesh),
	mName(name),
	mRootJoint(rootJoint),
	mAnimator(this),
	mAnimationVertexWeightsVBO(-1),
	mAnimationVertexJointIDsVBO(-1)
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
	mAnimator.BindAnimation(animation);
}

void AnimatedModel::FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const
{
	mRootJoint->FillWithJointTransforms(jointMatrices);
}

void AnimatedModel::BuildInternal(VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	int location = material->GetShader()->GetAttributeLocation("weights");
	if (location != -1)
	{
		CreateAnimationWeightsVBO(vertexBufferManager, location);
	}
	
	location = material->GetShader()->GetAttributeLocation("jointIndices");
	if (location != -1)
	{
		CreateAnimationVertexsJointIDsVBO(vertexBufferManager, location);
	}
}

void AnimatedModel::CreateAnimationWeightsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	long numVertexs = mMesh->GetNumberOfVertexs();
	if (numVertexs > 0)
	{
		// 1st attribute buffer : vertices
		std::string name("animation_weights_");
		name.append(std::to_string(GetID()));

		mAnimationVertexWeightsVBO = vertexBufferManager.CreateVBO(name);
		glBindBuffer(GL_ARRAY_BUFFER, mAnimationVertexWeightsVBO);

		std::vector<glm::vec4> data = mMesh->GetVertexsWeights();
		glBufferData(GL_ARRAY_BUFFER, numVertexs * sizeof(glm::vec4), &data[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,  // The attribute we want to configure
			4,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void AnimatedModel::CreateAnimationVertexsJointIDsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	long numVertexs = mMesh->GetNumberOfVertexs();
	if (numVertexs > 0)
	{
		// 1st attribute buffer : vertices
		std::string name("animation_vertex_joints_ids_");
		name.append(std::to_string(GetID()));

		mAnimationVertexJointIDsVBO = vertexBufferManager.CreateVBO(name);
		glBindBuffer(GL_ARRAY_BUFFER, mAnimationVertexJointIDsVBO);
		glBufferData(GL_ARRAY_BUFFER, numVertexs * sizeof(glm::ivec4), &mMesh->GetVertexsJointsIDs()[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(location);
		glVertexAttribIPointer(
			location,  // The attribute we want to configure
			4,                            // size
			GL_INT,                     // type
			GL_FALSE,                     // normalized?
			0
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}