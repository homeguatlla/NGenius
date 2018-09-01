#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

class Joint
{
public:
	Joint(int index, std::string& name, glm::mat4& localBindTransformation);
	~Joint();

	const std::string& GetName() const;
	const std::vector<Joint*>& GetJoints() const;
	void AddChild(Joint* joint);
	const glm::mat4& GetAnimatedTransform() const;
	void SetAnimatedTransform(const glm::mat4& animatedTransform);
	const glm::mat4& GetInverseBindTransform() const;
	void CalculateInverseBindTransform(const glm::mat4& parentBindTransform);
	void FillWithJointTransforms(std::vector<glm::mat4>& jointMatrices) const;

private:
	int mIndex;
	std::string mName;
	std::vector<Joint*> mChildren;
	glm::mat4 mLocalBindTransformation;
	glm::mat4 mInverseBindTransformation;
};

