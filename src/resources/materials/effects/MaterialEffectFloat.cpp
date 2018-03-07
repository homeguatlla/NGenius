#include "stdafx.h"
#include "MaterialEffectFloat.h"
#include "../IMaterial.h"

MaterialEffectFloat::MaterialEffectFloat(float* value) :
mValue(value)
{
}


MaterialEffectFloat::~MaterialEffectFloat()
{
}

float MaterialEffectFloat::GetFloat() const
{
	return *mValue;
}

void MaterialEffectFloat::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFloat* effect = material->GetEffect<MaterialEffectFloat>();
	if (effect != nullptr)
	{
		*mValue = effect->GetFloat();
	}

}

MaterialEffectFloat* MaterialEffectFloat::DoClone() const
{
	return new MaterialEffectFloat(*this);
}