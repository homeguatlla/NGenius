#include "stdafx.h"
#include "MaterialEffectFloat3Array.h"
#include "../IMaterial.h"

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
	return new MaterialEffectFloat3Array(*this);
}

IMaterialEffect* MaterialEffectFloat3Array::AddNewEffectToMaterial(IMaterial* material)
{
	MaterialEffectFloat3Array* materialEffect = new MaterialEffectFloat3Array();

	material->AddEffect(materialEffect);

	return materialEffect;
}

void MaterialEffectFloat3Array::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectFloat3Array::WriteTo(core::utils::ISerializer * destination)
{
}
