#pragma once
#include "glm/glm.hpp"
#include <vector>

class Joint;
class Model;

class AnimateModel
{
public:
	AnimateModel(const Model* model, Joint* rootJoint);
	~AnimateModel();

	const Model* GetModel() const;
	Joint* GetRootJoint();
	//void SetAnimation(const Animation* animation);
	
	void FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const;
	
	void Update();

private:
	//void CreateAnimationWeightsVBO(VertexBuffersManager& vertexBufferManager, int location);

private:
	//skeleton
	Joint* mRootJoint;

	//skin
	const Model* mModel;

	//Animator mAnimator;
};

