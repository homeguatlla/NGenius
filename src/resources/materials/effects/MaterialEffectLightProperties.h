#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class MaterialEffectLightProperties : public IMaterialEffect
{
	glm::vec3 mSunLightDirection;
	glm::vec3 mSunLightColor;

public:
	MaterialEffectLightProperties();
	virtual ~MaterialEffectLightProperties();

	void SetSunLightProperties(const glm::vec3& sunDirection, const glm::vec3& sunColor);
	glm::vec3 GetSunLightDirection() const;
	glm::vec3 GetSunLightColor() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectLightProperties* DoClone() const override;
};

