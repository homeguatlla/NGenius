#include "stdafx.h"
#include "MaterialEffectHeightMapTexture.h"
#include <assert.h>

MaterialEffectHeightMapTexture::MaterialEffectHeightMapTexture(ITexture* texture, float tile) :
mTexture(texture),
mTile(tile)
{
	assert(texture != nullptr);
}


MaterialEffectHeightMapTexture::~MaterialEffectHeightMapTexture()
{
}

ITexture* MaterialEffectHeightMapTexture::GetHeightMapTexture() const
{
	return mTexture;
}

float MaterialEffectHeightMapTexture::GetTile() const
{
	return mTile;
}

MaterialEffectHeightMapTexture* MaterialEffectHeightMapTexture::DoClone() const
{
	return new MaterialEffectHeightMapTexture(*this);
}