#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectClippingPlane : public IMaterialEffect
{
	glm::vec4 mPlane;

public:
	MaterialEffectClippingPlane();
	virtual ~MaterialEffectClippingPlane();

	void SetClippingPlane(const glm::vec4& plane);
	const glm::vec4 GetClippingPlane() const;

	MaterialEffectClippingPlane* DoClone() const override;
};

