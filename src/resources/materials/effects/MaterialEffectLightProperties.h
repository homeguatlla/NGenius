#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class MaterialEffectLightProperties : public IMaterialEffect
{
	glm::vec3 mPosition;
	glm::vec3 mColor;

public:
	MaterialEffectLightProperties(const glm::vec3& position, const glm::vec3& color);
	virtual ~MaterialEffectLightProperties();

	glm::vec3 GetPosition() const;
	glm::vec3 GetColor() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectLightProperties* DoClone() const override;
};

