#include "stdafx.h"
#include "MaterialEffectWater.h"
#include "../IMaterial.h"

MaterialEffectWater::MaterialEffectWater() :
	mReflectionTexture(nullptr),
	mRefractionTexture(nullptr),
	mDistorsionTexture(nullptr),
	mDepthTexture(nullptr),
	mNormalTexture(nullptr),
	mColor(1.0f),
	mSpeed(0.0f)
{
}

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

void MaterialEffectWater::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectWater* effect = material->GetEffect<MaterialEffectWater>();
	if (effect != nullptr)
	{
		mRefractionTexture = effect->GetRefractionTexture();
		mReflectionTexture = effect->GetReflectionTexture();
		mDistorsionTexture = effect->GetDistorsionTexture();
		mDepthTexture = effect->GetDepthTexture();
		mNormalTexture = effect->GetNormalTexture();
		mSpeed = effect->GetSpeed();
		mColor = effect->GetColor();
	}
}

MaterialEffectWater* MaterialEffectWater::DoClone() const
{
	return new MaterialEffectWater(*this);
}

IMaterialEffect* MaterialEffectWater::AddNewEffectToMaterial(IMaterial* material)
{
	MaterialEffectWater* materialEffect = new MaterialEffectWater();

	material->AddEffect(materialEffect);

	return materialEffect;
}

void MaterialEffectWater::ReadFrom(core::utils::IDeserializer * source)
{
}

void MaterialEffectWater::WriteTo(core::utils::ISerializer * destination)
{
}
