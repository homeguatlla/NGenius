#include "stdafx.h"
#include "MaterialEffectDepthTexture.h"
#include <assert.h>

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

MaterialEffectDepthTexture* MaterialEffectDepthTexture::DoClone() const
{
	return new MaterialEffectDepthTexture(*this);
}