#include "stdafx.h"
#include "MaterialEffectFloat2.h"

MaterialEffectFloat2::MaterialEffectFloat2(const glm::vec2& value) : mValue(value)
{
}


MaterialEffectFloat2::~MaterialEffectFloat2()
{
}

void MaterialEffectFloat2::SetValue(const glm::vec2& value)
{
	mValue = value;
}

const glm::vec2 MaterialEffectFloat2::GetValue() const
{
	return mValue;
}

MaterialEffectFloat2* MaterialEffectFloat2::DoClone() const
{
	return new MaterialEffectFloat2(*this);
}