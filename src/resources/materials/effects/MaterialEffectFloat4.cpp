#include "stdafx.h"
#include "MaterialEffectFloat4.h"
#include "../IMaterial.h"

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

void MaterialEffectFloat4::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFloat4* effect = material->GetEffect<MaterialEffectFloat4>();
	if (effect != nullptr)
	{
		mValue = effect->GetValue();
	}
}

MaterialEffectFloat4* MaterialEffectFloat4::DoClone() const
{
	return new MaterialEffectFloat4(*this);
}