#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class RotationComponent : public IComponent
{
	glm::vec3 mRotation;
	float mSpeed;
	float mCurrentAngle;

public:
	explicit RotationComponent(const glm::vec3& rotation, float speed);
	~RotationComponent();

	RotationComponent* DoClone() const override;
	void Update(float elapsedTime) override;
};

