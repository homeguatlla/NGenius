#include "stdafx.h"
#include "MaterialEffectFloat3.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"

MaterialEffectFloat3::MaterialEffectFloat3() :
	mValue(0.0f)
{
}

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

IMaterialEffect* MaterialEffectFloat3::AddNewEffectToMaterial(IMaterial* material)
{
	MaterialEffectFloat3* materialEffect = new MaterialEffectFloat3();

	material->AddEffect(materialEffect);

	return materialEffect;
}

void MaterialEffectFloat3::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("values", mValue);
}

void MaterialEffectFloat3::WriteTo(core::utils::ISerializer * destination)
{
}
