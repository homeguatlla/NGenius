#include "stdafx.h"
#include "MaterialEffectNormalTexture.h"
#include "../IMaterial.h"
#include "../../textures/TexturesLibrary.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"
#include "../Memory.h"

#include <assert.h>

MaterialEffectNormalTexture::MaterialEffectNormalTexture() :
	mTexture(nullptr),
	mTile(1.0f),
	mTextureName("")
{

}

MaterialEffectNormalTexture::MaterialEffectNormalTexture(ITexture* texture, float tile) :
mTexture(texture),
mTile(tile)
{
	assert(texture != nullptr);
}


MaterialEffectNormalTexture::~MaterialEffectNormalTexture()
{
}

ITexture* MaterialEffectNormalTexture::GetNormalTexture() const
{
	return mTexture;
}

float MaterialEffectNormalTexture::GetTile() const
{
	return mTile;
}

void MaterialEffectNormalTexture::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectNormalTexture* effect = material->GetEffect<MaterialEffectNormalTexture>();
	if (effect != nullptr)
	{
		mTile = effect->GetTile();
		mTexture = effect->GetNormalTexture();
	}
}

void MaterialEffectNormalTexture::Build(TexturesLibrary* texturesLibrary)
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

MaterialEffectNormalTexture* MaterialEffectNormalTexture::DoClone() const
{
	return DBG_NEW MaterialEffectNormalTexture(*this);
}

IMaterialEffect* MaterialEffectNormalTexture::Create(IMaterial* material)
{
	MaterialEffectNormalTexture* effect = DBG_NEW MaterialEffectNormalTexture();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectNormalTexture::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("texture", mTextureName);
	source->ReadParameter("tile", &mTile);
}

void MaterialEffectNormalTexture::WriteTo(core::utils::ISerializer * destination)
{
}
