#include "stdafx.h"
#include "MaterialEffectFloat3Array.h"
#include "../IMaterial.h"
#include "../Memory.h"

MaterialEffectFloat3Array::MaterialEffectFloat3Array()
{
}

MaterialEffectFloat3Array::MaterialEffectFloat3Array(const std::vector<glm::vec3>& values) : mValues(values)
{
}


MaterialEffectFloat3Array::~MaterialEffectFloat3Array()
{
}

void MaterialEffectFloat3Array::SetValues(const std::vector<glm::vec3>& values)
{
	mValues = values;
}

std::vector<glm::vec3>& MaterialEffectFloat3Array::GetValues()
{
	return mValues;
}

void MaterialEffectFloat3Array::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFloat3Array* effect = material->GetEffect<MaterialEffectFloat3Array>();
	if (effect != nullptr)
	{
		mValues = effect->GetValues();
	}
}

MaterialEffectFloat3Array* MaterialEffectFloat3Array::DoClone() const
{
	return DBG_NEW MaterialEffectFloat3Array(*this);
}

IMaterialEffect* MaterialEffectFloat3Array::Create(IMaterial* material)
{
	MaterialEffectFloat3Array* effect = DBG_NEW MaterialEffectFloat3Array();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectFloat3Array::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectFloat3Array::WriteTo(core::utils::ISerializer * destination)
{
}
