#include "stdafx.h"
#include "MaterialEffectClippingPlane.h"
#include "../IMaterial.h"

MaterialEffectClippingPlane::MaterialEffectClippingPlane() :
	mPlane(glm::vec4(0.0f))
{
}


MaterialEffectClippingPlane::~MaterialEffectClippingPlane()
{
}

void MaterialEffectClippingPlane::SetClippingPlane(const glm::vec4& plane)
{
	mPlane = plane;
}

const glm::vec4 MaterialEffectClippingPlane::GetClippingPlane() const
{
	return mPlane;
}

void MaterialEffectClippingPlane::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectClippingPlane* effect = material->GetEffect<MaterialEffectClippingPlane>();
	if (effect != nullptr)
	{
		mPlane = effect->GetClippingPlane();
	}
}

MaterialEffectClippingPlane* MaterialEffectClippingPlane::DoClone() const
{
	return new MaterialEffectClippingPlane(*this);
}

IMaterialEffect* MaterialEffectClippingPlane::Create(IMaterial* material)
{
	MaterialEffectClippingPlane* effect = new MaterialEffectClippingPlane();
	material->AddEffect(effect);
	return effect;
}

void MaterialEffectClippingPlane::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectClippingPlane::WriteTo(core::utils::ISerializer * destination)
{
}
