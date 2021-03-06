#include "stdafx.h"
#include "MaterialEffectShadowProperties.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"

MaterialEffectShadowProperties::MaterialEffectShadowProperties(int pfcCounter) :
	mDepthTexture(nullptr),
	mPFCCounter(pfcCounter),
	mMatrix(glm::mat4(0.0f))
{
}


MaterialEffectShadowProperties::~MaterialEffectShadowProperties()
{
}

void MaterialEffectShadowProperties::SetParameters(const ITexture* texture, const glm::mat4& matrix, int pfcCounter)
{
	mDepthTexture = texture;
	mMatrix = matrix;
	mPFCCounter = pfcCounter;
}

const ITexture* MaterialEffectShadowProperties::GetDepthTexture() const
{
	return mDepthTexture;
}

int MaterialEffectShadowProperties::GetPFCCounter() const
{
	return mPFCCounter;
}

const glm::mat4& MaterialEffectShadowProperties::GetMatrix() const
{
	return mMatrix;
}

void MaterialEffectShadowProperties::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectShadowProperties* effect = material->GetEffect<MaterialEffectShadowProperties>();
	if (effect != nullptr)
	{
		mMatrix = effect->GetMatrix();
		mPFCCounter = effect->GetPFCCounter();
		mDepthTexture = effect->GetDepthTexture();
	}
}

MaterialEffectShadowProperties* MaterialEffectShadowProperties::DoClone() const
{
	return DBG_NEW MaterialEffectShadowProperties(*this);
}

IMaterialEffect* MaterialEffectShadowProperties::Create(IMaterial* material)
{
	MaterialEffectShadowProperties* effect = DBG_NEW MaterialEffectShadowProperties();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectShadowProperties::ReadFrom(core::utils::IDeserializer *source)
{
	source->ReadParameter("pfc_counter", &mPFCCounter);
}

void MaterialEffectShadowProperties::WriteTo(core::utils::ISerializer *destination)
{
}
