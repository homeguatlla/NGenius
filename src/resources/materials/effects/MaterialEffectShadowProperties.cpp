#include "stdafx.h"
#include "MaterialEffectShadowProperties.h"

MaterialEffectShadowProperties::MaterialEffectShadowProperties() 
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

MaterialEffectShadowProperties* MaterialEffectShadowProperties::DoClone() const
{
	return new MaterialEffectShadowProperties(*this);
}