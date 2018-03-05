#include "stdafx.h"
#include "MaterialEffectFogProperties.h"

MaterialEffectFogProperties::MaterialEffectFogProperties(const glm::vec3& color, float density, float gradient) :
	mColor(color),
	mDensity(density),
	mGradient(gradient)
{
}


MaterialEffectFogProperties::~MaterialEffectFogProperties()
{
}

void MaterialEffectFogProperties::SetProperties(const glm::vec3& color, float density, float gradient)
{
	mColor = color;
	mDensity = density;
	mGradient = gradient;
}

const glm::vec3 MaterialEffectFogProperties::GetColor() const
{
	return mColor;
}

float MaterialEffectFogProperties::GetDensity() const
{
	return mDensity;
}

float MaterialEffectFogProperties::GetGradient() const
{
	return mGradient;
}

MaterialEffectFogProperties* MaterialEffectFogProperties::DoClone() const
{
	return new MaterialEffectFogProperties(*this);
}