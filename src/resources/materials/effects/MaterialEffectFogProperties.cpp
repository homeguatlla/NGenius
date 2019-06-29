#include "stdafx.h"
#include "MaterialEffectFogProperties.h"
#include "../IMaterial.h"

MaterialEffectFogProperties::MaterialEffectFogProperties() :
	mColor(glm::vec3(0.0f)),
	mDensity(0.0f),
	mGradient(0.0f)
{
}


MaterialEffectFogProperties::~MaterialEffectFogProperties()
{
}

void MaterialEffectFogProperties::SetProperties(const glm::vec3& color, float density, float gradient)
{
	mColor = color;
	mDensity = density;
	mGradient = gradient;
}

const glm::vec3 MaterialEffectFogProperties::GetColor() const
{
	return mColor;
}

float MaterialEffectFogProperties::GetDensity() const
{
	return mDensity;
}

float MaterialEffectFogProperties::GetGradient() const
{
	return mGradient;
}

void MaterialEffectFogProperties::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectFogProperties* effect = material->GetEffect<MaterialEffectFogProperties>();
	if (effect != nullptr)
	{
		mColor = effect->GetColor();
		mDensity = effect->GetDensity();
		mGradient = effect->GetGradient();
	}
}

MaterialEffectFogProperties* MaterialEffectFogProperties::DoClone() const
{
	return new MaterialEffectFogProperties(*this);
}

void MaterialEffectFogProperties::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectFogProperties::WriteTo(core::utils::ISerializer * destination)
{
}
