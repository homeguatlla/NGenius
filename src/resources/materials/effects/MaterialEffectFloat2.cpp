#include "stdafx.h"
#include "MaterialEffectFloat2.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"

MaterialEffectFloat2::MaterialEffectFloat2() :
	mValue(0.0f)
{
}

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

void MaterialEffectFloat2::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFloat2* effect = material->GetEffect<MaterialEffectFloat2>();
	if (effect != nullptr)
	{
		mValue = effect->GetValue();
	}
}

MaterialEffectFloat2* MaterialEffectFloat2::DoClone() const
{
	return new MaterialEffectFloat2(*this);
}

void MaterialEffectFloat2::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("values", mValue);
}

void MaterialEffectFloat2::WriteTo(core::utils::ISerializer * destination)
{
}
