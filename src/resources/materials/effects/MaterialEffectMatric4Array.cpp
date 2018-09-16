#include "stdafx.h"
#include "MaterialEffectMatrix4Array.h"
#include "../IMaterial.h"

MaterialEffectMatrix4Array::MaterialEffectMatrix4Array()
{
}

MaterialEffectMatrix4Array::MaterialEffectMatrix4Array(const std::vector<glm::mat4>& values) : mValues(values)
{
}


MaterialEffectMatrix4Array::~MaterialEffectMatrix4Array()
{
}

void MaterialEffectMatrix4Array::SetValues(const std::vector<glm::mat4>& values)
{
	mValues = values;
}

std::vector<glm::mat4>& MaterialEffectMatrix4Array::GetValues()
{
	return mValues;
}

void MaterialEffectMatrix4Array::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectMatrix4Array* effect = material->GetEffect<MaterialEffectMatrix4Array>();
	if (effect != nullptr)
	{
		mValues = effect->GetValues();
	}
}

MaterialEffectMatrix4Array* MaterialEffectMatrix4Array::DoClone() const
{
	return new MaterialEffectMatrix4Array(*this);
}