#include "stdafx.h"
#include "MaterialEffectFloat4.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"

MaterialEffectFloat4::MaterialEffectFloat4() :
	mValue(0.0f)
{
}

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

IMaterialEffect* MaterialEffectFloat4::Create(IMaterial* material)
{
	MaterialEffectFloat4* effect = new MaterialEffectFloat4();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectFloat4::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("values", mValue);
}

void MaterialEffectFloat4::WriteTo(core::utils::ISerializer * destination)
{
}
