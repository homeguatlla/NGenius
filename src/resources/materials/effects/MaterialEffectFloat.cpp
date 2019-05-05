#include "stdafx.h"
#include "MaterialEffectFloat.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"

MaterialEffectFloat::MaterialEffectFloat()
	: mValue(0.0f)
{
}

MaterialEffectFloat::MaterialEffectFloat(float value) :
mValue(value)
{
}


MaterialEffectFloat::~MaterialEffectFloat()
{
}

float MaterialEffectFloat::GetValue() const
{
	return mValue;
}

void MaterialEffectFloat::SetValue(float value)
{
	mValue = value;
}

void MaterialEffectFloat::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFloat* effect = material->GetEffect<MaterialEffectFloat>();
	if (effect != nullptr)
	{
		mValue = effect->GetValue();
	}

}

MaterialEffectFloat* MaterialEffectFloat::DoClone() const
{
	return new MaterialEffectFloat(*this);
}

IMaterialEffect* MaterialEffectFloat::AddNewEffectToMaterial(IMaterial* material)
{
	MaterialEffectFloat* materialEffect = new MaterialEffectFloat();

	material->AddEffect(materialEffect);

	return materialEffect;
}

void MaterialEffectFloat::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("values", &mValue);
}

void MaterialEffectFloat::WriteTo(core::utils::ISerializer * destination)
{
}
