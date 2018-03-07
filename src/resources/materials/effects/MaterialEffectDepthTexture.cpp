#include "stdafx.h"
#include "MaterialEffectDepthTexture.h"
#include "../IMaterial.h"
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
	return new MaterialEffectDepthTexture(*this);
}