#include "stdafx.h"
#include "MaterialEffectFloat3.h"

MaterialEffectFloat3::MaterialEffectFloat3(const glm::vec3& value) : mValue(value)
{
}


MaterialEffectFloat3::~MaterialEffectFloat3()
{
}

void MaterialEffectFloat3::SetValue(const glm::vec3& value)
{
	mValue = value;
}

const glm::vec3 MaterialEffectFloat3::GetValue() const
{
	return mValue;
}

MaterialEffectFloat3* MaterialEffectFloat3::DoClone() const
{
	return new MaterialEffectFloat3(*this);
}