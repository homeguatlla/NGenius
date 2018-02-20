#include "stdafx.h"
#include "MaterialEffectHeightMapTexture.h"

MaterialEffectHeightMapTexture::MaterialEffectHeightMapTexture(ITexture* texture, float tile) :
mTexture(texture),
mTile(tile)
{
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