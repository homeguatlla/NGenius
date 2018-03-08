#include "stdafx.h"
#include "MaterialEffectFloat.h"
#include "../IMaterial.h"

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