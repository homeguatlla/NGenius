#include "stdafx.h"
#include "MaterialEffectFloat.h"

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

MaterialEffectFloat* MaterialEffectFloat::DoClone() const
{
	return new MaterialEffectFloat(*this);
}