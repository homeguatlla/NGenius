#include "stdafx.h"
#include "MaterialEffectNormalTexture.h"
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

float MaterialEffectNormalTexture::GetTile() const
{
	return mTile;
}

MaterialEffectNormalTexture* MaterialEffectNormalTexture::DoClone() const
{
	return new MaterialEffectNormalTexture(*this);
}