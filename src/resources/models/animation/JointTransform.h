#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class JointTransform
{
public:
	JointTransform(glm::vec3& position, glm::quat& rotation);
	JointTransform(const glm::mat4x4& localTransformation);
	~JointTransform();

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	const glm::mat4x4& GetLocalTransform() const;
	void SetLocalTransform(const glm::mat4x4& matrix);

	static JointTransform Interpolate(JointTransform* frameA, JointTransform* frameB, float progression);

private:
	void CalculateLocalTransform();
	void CalculatePositionRotation();
	glm::quat FromMatrix(const glm::mat4x4& matrix);
	glm::mat4x4& ToRotationMatrix();

private:
	glm::vec3 mPosition;
	glm::quat mRotation;

	glm::mat4x4 mLocalTransform;
};

