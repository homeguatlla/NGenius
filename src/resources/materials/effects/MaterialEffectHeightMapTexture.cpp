#include "stdafx.h"
#include "MaterialEffectHeightMapTexture.h"
#include "../IMaterial.h"
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

void MaterialEffectHeightMapTexture::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectHeightMapTexture* effect = material->GetEffect<MaterialEffectHeightMapTexture>();
	if (effect != nullptr)
	{
		mTexture = effect->GetHeightMapTexture();
		mTile = effect->GetTile();
	}
}

MaterialEffectHeightMapTexture* MaterialEffectHeightMapTexture::DoClone() const
{
	return new MaterialEffectHeightMapTexture(*this);
}