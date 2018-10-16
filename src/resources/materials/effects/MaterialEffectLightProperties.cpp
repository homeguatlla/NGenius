#include "stdafx.h"
#include "MaterialEffectLightProperties.h"
#include "../IMaterial.h"

MaterialEffectLightProperties::MaterialEffectLightProperties() :
mSunLightDirection(glm::vec3(10000.0f, 10000.0f, 10000.0f)),
mSunLightColor(glm::vec3(1.0f))
{
}


MaterialEffectLightProperties::~MaterialEffectLightProperties()
{
}

void MaterialEffectLightProperties::SetSunLightProperties(const glm::vec3& sunDirection, const glm::vec3& sunColor)
{
	mSunLightDirection = sunDirection;
	mSunLightColor = sunColor;
}

glm::vec3 MaterialEffectLightProperties::GetSunLightDirection() const
{
	return mSunLightDirection;
}

glm::vec3 MaterialEffectLightProperties::GetSunLightColor() const
{
	return mSunLightColor;
}

void MaterialEffectLightProperties::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectLightProperties* effect = material->GetEffect<MaterialEffectLightProperties>();
	if (effect != nullptr)
	{
		mSunLightDirection = effect->GetSunLightDirection();
		mSunLightColor = effect->GetSunLightColor();
	}
}

MaterialEffectLightProperties* MaterialEffectLightProperties::DoClone() const
{
	return new MaterialEffectLightProperties(*this);
}