#include "stdafx.h"
#include "MaterialEffectDepthTexture.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"
#include "../../textures/TexturesLibrary.h"
#include "../Memory.h"

#include <assert.h>

MaterialEffectDepthTexture::MaterialEffectDepthTexture() :
	mTexture(nullptr),
	mTile(1.0f)
{
}

MaterialEffectDepthTexture::MaterialEffectDepthTexture(ITexture* texture, float tile) :
mTexture(texture),
mTile(tile)
{
	assert(texture != nullptr);
}


MaterialEffectDepthTexture::~MaterialEffectDepthTexture()
{
}

ITexture* MaterialEffectDepthTexture::GetDepthTexture() const
{
	return mTexture;
}

float MaterialEffectDepthTexture::GetTile() const
{
	return mTile;
}

void MaterialEffectDepthTexture::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectDepthTexture* effect = material->GetEffect<MaterialEffectDepthTexture>();
	if (effect != nullptr)
	{
		mTexture = effect->GetDepthTexture();
		mTile = effect->GetTile();
	}
}

MaterialEffectDepthTexture* MaterialEffectDepthTexture::DoClone() const
{
	return DBG_NEW MaterialEffectDepthTexture(*this);
}

IMaterialEffect* MaterialEffectDepthTexture::Create(IMaterial* material)
{
	MaterialEffectDepthTexture* effect = DBG_NEW MaterialEffectDepthTexture();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectDepthTexture::Build(TexturesLibrary* texturesLibrary)
{
	if (!mTextureName.empty())
	{
		mTexture = texturesLibrary->GetElement(mTextureName);
		if (mTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
}

void MaterialEffectDepthTexture::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("texture", mTextureName);
	source->ReadParameter("tile", &mTile);
}

void MaterialEffectDepthTexture::WriteTo(core::utils::ISerializer * destination)
{
}
