#include "stdafx.h"
#include "MaterialEffectDirectionalLightProperties.h"
#include "../IMaterial.h"
#include "../../GameConstants.h"

MaterialEffectDirectionalLightProperties::MaterialEffectDirectionalLightProperties() :
mSunLightDirection(SUN_POSITION_DEFAULT),
mSunLightColor(glm::vec3(1.0f))
{
}


MaterialEffectDirectionalLightProperties::~MaterialEffectDirectionalLightProperties()
{
}

void MaterialEffectDirectionalLightProperties::SetSunLightProperties(const glm::vec3& sunDirection, const glm::vec3& sunColor)
{
	mSunLightDirection = sunDirection;
	mSunLightColor = sunColor;
}

const glm::vec3& MaterialEffectDirectionalLightProperties::GetSunLightDirection() const
{
	return mSunLightDirection;
}

const glm::vec3& MaterialEffectDirectionalLightProperties::GetSunLightColor() const
{
	return mSunLightColor;
}

void MaterialEffectDirectionalLightProperties::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectDirectionalLightProperties* effect = material->GetEffect<MaterialEffectDirectionalLightProperties>();
	if (effect != nullptr)
	{
		mSunLightDirection = effect->GetSunLightDirection();
		mSunLightColor = effect->GetSunLightColor();
	}
}

MaterialEffectDirectionalLightProperties* MaterialEffectDirectionalLightProperties::DoClone() const
{
	return new MaterialEffectDirectionalLightProperties(*this);
}

IMaterialEffect* MaterialEffectDirectionalLightProperties::Create(IMaterial* material)
{
	MaterialEffectDirectionalLightProperties* effect = new MaterialEffectDirectionalLightProperties();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectDirectionalLightProperties::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectDirectionalLightProperties::WriteTo(core::utils::ISerializer * destination)
{
}
