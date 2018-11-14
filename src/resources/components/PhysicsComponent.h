#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class PhysicsComponent : public IComponent
{
	bool mIsStatic;
	glm::vec3 mVelocity;
	const glm::vec3 mGravity;
	float mIntroductionCoef; //coeficiente de penetración, offset en Y de penetración

public:
	PhysicsComponent(bool isStatic, const glm::vec3& gravity, float introductionCoef = 0.0f);
	~PhysicsComponent();

	PhysicsComponent* DoClone() const override;

	const glm::vec3 GetVelocity() const;
	void SetVelocity(glm::vec3& velocity);
	const glm::vec3 GetGravity() const;
	bool IsStatic() const;
	float GetIndroductionCoef() const;
};

