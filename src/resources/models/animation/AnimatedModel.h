#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "Animator.h"

#include <string>

class Joint;
class Model;

class AnimatedModel
{
public:
	AnimatedModel(const std::string& name, const Model* model, Joint* rootJoint);
	~AnimatedModel();

	const Model* GetModel() const;
	Joint* GetRootJoint();
	std::string GetName() const;
	void PlayAnimation(Animation* animation);
	
	void FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const;
	
	void Update(float elapsedTime);

private:
	//void CreateAnimationWeightsVBO(VertexBuffersManager& vertexBufferManager, int location);

private:
	std::string mName;

	//skeleton
	Joint* mRootJoint;

	//skin
	const Model* mModel;

	Animator mAnimator;
};

