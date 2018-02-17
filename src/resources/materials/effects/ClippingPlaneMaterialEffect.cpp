#include "stdafx.h"
#include "ClippingPlaneMaterialEffect.h"

ClippingPlaneMaterialEffect::ClippingPlaneMaterialEffect()
{
}


ClippingPlaneMaterialEffect::~ClippingPlaneMaterialEffect()
{
}

void ClippingPlaneMaterialEffect::SetClippingPlane(const glm::vec4& plane)
{
	mPlane = plane;
}

const glm::vec4 ClippingPlaneMaterialEffect::GetClippingPlane() const
{
	return mPlane;
}

ClippingPlaneMaterialEffect* ClippingPlaneMaterialEffect::DoClone() const
{
	return new ClippingPlaneMaterialEffect(*this);
}