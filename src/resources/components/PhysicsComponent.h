#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class PhysicsComponent : public IComponent
{
	bool mIsStatic;
	glm::vec3 mVelocity;
	const glm::vec3 mGravity;

public:
	PhysicsComponent(bool isStatic, const glm::vec3& gravity);
	~PhysicsComponent();

	const glm::vec3 GetVelocity() const;
	void SetVelocity(glm::vec3& velocity);
	const glm::vec3 GetGravity() const;
	bool IsStatic() const;

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	PhysicsComponent* DoClone() const override;
};

