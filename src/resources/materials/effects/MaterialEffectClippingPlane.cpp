#include "stdafx.h"
#include "MaterialEffectClippingPlane.h"

MaterialEffectClippingPlane::MaterialEffectClippingPlane()
{
}


MaterialEffectClippingPlane::~MaterialEffectClippingPlane()
{
}

void MaterialEffectClippingPlane::SetClippingPlane(const glm::vec4& plane)
{
	mPlane = plane;
}

const glm::vec4 MaterialEffectClippingPlane::GetClippingPlane() const
{
	return mPlane;
}

MaterialEffectClippingPlane* MaterialEffectClippingPlane::DoClone() const
{
	return new MaterialEffectClippingPlane(*this);
}