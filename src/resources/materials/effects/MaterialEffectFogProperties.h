#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectFogProperties : public IMaterialEffect
{
	glm::vec3 mColor;
	float mDensity;
	float mGradient;

public:
	MaterialEffectFogProperties(const glm::vec3& color, float density, float gradient);
	virtual ~MaterialEffectFogProperties();

	void SetProperties(const glm::vec3& color, float density, float gradient);

	const glm::vec3 GetColor() const;
	float GetDensity() const;
	float GetGradient() const;

	MaterialEffectFogProperties* DoClone() const override;
};

