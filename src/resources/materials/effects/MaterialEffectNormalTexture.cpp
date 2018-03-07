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

MaterialEffectNormalTexture* MaterialEffectNormalTexture::DoClone() const
{
	return new MaterialEffectNormalTexture(*this);
}