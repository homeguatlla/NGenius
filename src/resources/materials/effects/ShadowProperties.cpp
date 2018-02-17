#include "stdafx.h"
#include "ShadowProperties.h"

ShadowProperties::ShadowProperties() 
{
}


ShadowProperties::~ShadowProperties()
{
}

void ShadowProperties::SetParameters(ITexture* texture, const glm::mat4& matrix, int pfcCounter)
{
	mDepthTexture = texture;
	mMatrix = matrix;
	mPFCCounter = pfcCounter;
}

ITexture* ShadowProperties::GetDepthTexture() const
{
	return mDepthTexture;
}

int ShadowProperties::GetPFCCounter() const
{
	return mPFCCounter;
}

const glm::mat4& ShadowProperties::GetMatrix() const
{
	return mMatrix;
}

ShadowProperties* ShadowProperties::DoClone() const
{
	return new ShadowProperties(*this);
}