#include "stdafx.h"
#include "MaterialEffectFloat4.h"

MaterialEffectFloat4::MaterialEffectFloat4(const glm::vec4& value) : mValue(value)
{
}


MaterialEffectFloat4::~MaterialEffectFloat4()
{
}

void MaterialEffectFloat4::SetValue(const glm::vec4& value)
{
	mValue = value;
}

const glm::vec4 MaterialEffectFloat4::GetValue() const
{
	return mValue;
}

MaterialEffectFloat4* MaterialEffectFloat4::DoClone() const
{
	return new MaterialEffectFloat4(*this);
}