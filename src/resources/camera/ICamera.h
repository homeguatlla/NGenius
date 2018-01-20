#pragma once
#include <glm/glm.hpp>

class ICamera
{
public:
	virtual ~ICamera();

	const glm::mat4& GetViewMatrix();
	const glm::mat4& GetProjectionMatrix() const;
	void SetPosition(const glm::vec3& position);
	void SetTarget(const glm::vec3& target);
	void SetUp(const glm::vec3& up);

	glm::vec3 GetPosition() const;
	glm::vec3 GetTarget() const;
	glm::vec3 GetUp() const;

	void Move(float speed);
	void Rotate(float angle, const glm::vec3& axis);

private:
	virtual void CreateViewMatrix() = 0;
	virtual void CreateProjectionMatrix() = 0;

protected:
	bool mIsDirty;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	glm::vec3 mPosition;
};

