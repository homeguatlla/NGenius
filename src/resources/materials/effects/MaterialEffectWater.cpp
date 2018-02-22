#include "stdafx.h"
#include "MaterialEffectWater.h"

MaterialEffectWater::MaterialEffectWater(ITexture* reflectionTexture, ITexture* refractionTexture,
	ITexture* distorsionTexture, ITexture* normalTexture, ITexture* depthTexture, float waterSpeed, const glm::vec4& waterColor) :
mReflectionTexture(reflectionTexture),
mRefractionTexture(refractionTexture),
mDistorsionTexture(distorsionTexture),
mNormalTexture(normalTexture),
mDepthTexture(depthTexture),
mSpeed(waterSpeed),
mColor(waterColor)
{
}

MaterialEffectWater::~MaterialEffectWater()
{
}

ITexture* MaterialEffectWater::GetReflectionTexture() const
{
	return mReflectionTexture;
}

ITexture* MaterialEffectWater::GetRefractionTexture() const
{
	return mRefractionTexture;
}

ITexture* MaterialEffectWater::GetDepthTexture() const
{
	return mDepthTexture;
}

ITexture* MaterialEffectWater::GetDistorsionTexture() const
{
	return mDistorsionTexture;
}

ITexture* MaterialEffectWater::GetNormalTexture() const
{
	return mNormalTexture;
}

const glm::vec4 MaterialEffectWater::GetColor() const
{
	return mColor;
}

float MaterialEffectWater::GetSpeed() const
{
	return mSpeed;
}

void MaterialEffectWater::SetSpeed(float speed)
{
	mSpeed = speed;
}

MaterialEffectWater* MaterialEffectWater::DoClone() const
{
	return new MaterialEffectWater(*this);
}