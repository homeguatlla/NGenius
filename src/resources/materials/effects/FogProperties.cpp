#include "stdafx.h"
#include "FogProperties.h"

FogProperties::FogProperties(const glm::vec3& color, float density, float gradient) :
	mColor(color),
	mDensity(density),
	mGradient(gradient)
{
}


FogProperties::~FogProperties()
{
}

void FogProperties::SetProperties(const glm::vec3& color, float density, float gradient)
{
	mColor = color;
	mDensity = density;
	mGradient = gradient;
}

const glm::vec3 FogProperties::GetColor() const
{
	return mColor;
}

float FogProperties::GetDensity() const
{
	return mDensity;
}

float FogProperties::GetGradient() const
{
	return mGradient;
}

FogProperties* FogProperties::DoClone() const
{
	return new FogProperties(*this);
}