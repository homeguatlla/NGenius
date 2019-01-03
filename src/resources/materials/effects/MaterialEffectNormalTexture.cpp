#include "stdafx.h"
#include "MaterialEffectNormalTexture.h"
#include "../IMaterial.h"
#include <assert.h>

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

void MaterialEffectNormalTexture::SetNormalTexture(ITexture* texture)
{
	assert(texture != nullptr);
	mTexture = texture;
}

float MaterialEffectNormalTexture::GetTile() const
{
	return mTile;
}

void MaterialEffectNormalTexture::SetTile(float tile)
{
	mTile = tile;
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

MaterialEffectNormalTexture* MaterialEffectNormalTexture::DoClone() const
{
	return new MaterialEffectNormalTexture(*this);
}