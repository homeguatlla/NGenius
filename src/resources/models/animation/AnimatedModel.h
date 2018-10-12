#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "Animator.h"
#include "../Model.h"

#include <string>

class Joint;
class Mesh;

class AnimatedModel : public Model
{
public:
	AnimatedModel(const std::string& name, Mesh* model, Joint* rootJoint);
	~AnimatedModel();

	Joint* GetRootJoint();
	std::string GetName() const;
	void PlayAnimation(Animation* animation);
	
	void FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const;
	
	bool IsAnimatedModel() const override;

private:
	void BuildInternal(VertexBuffersManager& vertexBufferManager, IMaterial* material) override;

	void CreateAnimationWeightsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateAnimationVertexsJointIDsVBO(VertexBuffersManager& vertexBufferManager, int location);
	
private:
	std::string mName;

	//skeleton
	Joint* mRootJoint;

	Animator mAnimator;

	int mAnimationVertexWeightsVBO;
	int mAnimationVertexJointIDsVBO;
};

