#include "stdafx.h"
#include "MaterialEffectLightProperties.h"

MaterialEffectLightProperties::MaterialEffectLightProperties(const glm::vec3& position, const glm::vec3& color) :
mPosition(position),
mColor(color)
{
}


MaterialEffectLightProperties::~MaterialEffectLightProperties()
{
}

glm::vec3 MaterialEffectLightProperties::GetPosition() const
{
	return mPosition;
}

glm::vec3 MaterialEffectLightProperties::GetColor() const
{
	return mColor;
}

MaterialEffectLightProperties* MaterialEffectLightProperties::DoClone() const
{
	return new MaterialEffectLightProperties(*this);
}