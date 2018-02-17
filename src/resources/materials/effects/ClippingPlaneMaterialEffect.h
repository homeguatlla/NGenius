#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class ClippingPlaneMaterialEffect : public IMaterialEffect
{
	glm::vec4 mPlane;

public:
	ClippingPlaneMaterialEffect();
	virtual ~ClippingPlaneMaterialEffect();

	void SetClippingPlane(const glm::vec4& plane);
	const glm::vec4 GetClippingPlane() const;

	ClippingPlaneMaterialEffect* DoClone() const override;
};

