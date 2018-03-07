#include "stdafx.h"
#include "MaterialEffectFloat3.h"
#include "../IMaterial.h"

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

void MaterialEffectFloat3::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFloat3* effect = material->GetEffect<MaterialEffectFloat3>();
	if (effect != nullptr)
	{
		mValue = effect->GetValue();
	}
}

MaterialEffectFloat3* MaterialEffectFloat3::DoClone() const
{
	return new MaterialEffectFloat3(*this);
}