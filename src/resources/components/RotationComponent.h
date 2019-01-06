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

	void UpdateInternal(float elapsedTime) override;

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	RotationComponent* DoClone() const override;
};

