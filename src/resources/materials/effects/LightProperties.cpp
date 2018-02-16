#include "stdafx.h"
#include "LightProperties.h"

LightProperties::LightProperties(const glm::vec3& position, const glm::vec3& color) :
mPosition(position),
mColor(color)
{
}


LightProperties::~LightProperties()
{
}

glm::vec3 LightProperties::GetPosition() const
{
	return mPosition;
}

glm::vec3 LightProperties::GetColor() const
{
	return mColor;
}

LightProperties* LightProperties::DoClone() const
{
	return new LightProperties(*this);
}