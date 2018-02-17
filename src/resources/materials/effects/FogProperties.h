#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class FogProperties : public IMaterialEffect
{
	glm::vec3 mColor;
	float mDensity;
	float mGradient;

public:
	FogProperties(const glm::vec3& color, float density, float gradient);
	virtual ~FogProperties();

	void SetProperties(const glm::vec3& color, float density, float gradient);

	const glm::vec3 GetColor() const;
	float GetDensity() const;
	float GetGradient() const;

	FogProperties* DoClone() const override;
};

