#include "stdafx.h"
#include "MaterialEffectMatrix4Array.h"
#include "../IMaterial.h"
#include "../Memory.h"

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
	mValues.clear();

	MaterialEffectMatrix4Array* effect = material->GetEffect<MaterialEffectMatrix4Array>();
	if (effect != nullptr)
	{
		mValues = effect->GetValues();
	}
}

MaterialEffectMatrix4Array* MaterialEffectMatrix4Array::DoClone() const
{
	return DBG_NEW MaterialEffectMatrix4Array(*this);
}

IMaterialEffect* MaterialEffectMatrix4Array::Create(IMaterial* material)
{
	MaterialEffectMatrix4Array* effect = DBG_NEW MaterialEffectMatrix4Array();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectMatrix4Array::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectMatrix4Array::WriteTo(core::utils::ISerializer * destination)
{
}
