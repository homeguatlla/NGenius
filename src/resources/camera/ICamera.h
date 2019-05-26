#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../../AABB.h"
#include "../../Frustum.h"
#include "../../utils/serializer/ISerializable.h"
#include "../IFactory.h"

class ICamera : public core::utils::ISerializable, public IFactory
{
public:
	virtual ~ICamera();

	const glm::mat4& GetViewMatrix();
	const glm::mat4& GetProjectionMatrix() const;
	void SetPosition(const glm::vec3& position);
	void SetTarget(const glm::vec3& target);
	void SetUp(const glm::vec3& up);
	void SetName(const std::string& name);
	void SetFrustumDilatation(float value);

	glm::vec3 GetPosition() const;
	glm::vec3 GetTarget() const;
	glm::vec3 GetUp() const;
	const std::string& GetName() const;
	const Frustum& GetFrustum();

	void Move(float speed);
	void Rotate(float angle, const glm::vec3& axis);
	virtual AABB GetAABB() const = 0;
	virtual void CalculateFrustum() = 0;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

private:
	void UpdateDirty();
	virtual void CreateViewMatrix() = 0;
	virtual void CreateProjectionMatrix() = 0;

protected:
	std::string mName;
	bool mIsDirty;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	glm::vec3 mPosition;

	Frustum mFrustum;
	float mFrustumDilatation;

	
};

