#include "stdafx.h"
#include "MaterialEffectWater.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"
#include "../../textures/TexturesLibrary.h"
#include "../Memory.h"

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
	return DBG_NEW MaterialEffectWater(*this);
}

IMaterialEffect* MaterialEffectWater::Create(IMaterial* material)
{
	MaterialEffectWater* effect = DBG_NEW MaterialEffectWater();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectWater::Build(TexturesLibrary* texturesLibrary)
{
	if (!mReflectionTextureName.empty())
	{
		mReflectionTexture = texturesLibrary->GetElement(mReflectionTextureName);
		if (mReflectionTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mReflectionTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
	if (!mRefractionTextureName.empty())
	{
		mRefractionTexture = texturesLibrary->GetElement(mRefractionTextureName);
		if (mRefractionTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mRefractionTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
	if (!mDistorsionTextureName.empty())
	{
		mDistorsionTexture = texturesLibrary->GetElement(mDistorsionTextureName);
		if (mDistorsionTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mDistorsionTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
	if (!mNormalTextureName.empty())
	{
		mNormalTexture = texturesLibrary->GetElement(mNormalTextureName);
		if (mNormalTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mNormalTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
	if (!mDepthTextureName.empty())
	{
		mDepthTexture = texturesLibrary->GetElement(mDepthTextureName);
		if (mDepthTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mDepthTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
}

void MaterialEffectWater::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("reflection_texture", mReflectionTextureName);
	source->ReadParameter("refraction_texture", mRefractionTextureName);
	source->ReadParameter("distorsion_texture", mDistorsionTextureName);
	source->ReadParameter("normal_texture", mNormalTextureName);
	source->ReadParameter("depth_texture", mDepthTextureName);
	source->ReadParameter("color", mColor);
	source->ReadParameter("speed", &mSpeed);
}

void MaterialEffectWater::WriteTo(core::utils::ISerializer * destination)
{
}
