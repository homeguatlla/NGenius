#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class LightProperties : public IMaterialEffect
{
	glm::vec3 mPosition;
	glm::vec3 mColor;

public:
	LightProperties(const glm::vec3& position, const glm::vec3& color);
	virtual ~LightProperties();

	glm::vec3 GetPosition() const;
	glm::vec3 GetColor() const;

	LightProperties* DoClone() const override;
};

