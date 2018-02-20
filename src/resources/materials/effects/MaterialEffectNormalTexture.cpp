#include "stdafx.h"
#include "MaterialEffectNormalTexture.h"

MaterialEffectNormalTexture::MaterialEffectNormalTexture(ITexture* texture, float tile) :
mTexture(texture),
mTile(tile)
{
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