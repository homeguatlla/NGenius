#include "stdafx.h"
#include "MaterialEffectDiffuseTexture.h"

MaterialEffectDiffuseTexture::MaterialEffectDiffuseTexture(ITexture* texture, const glm::vec3& color, float tile) :
mTexture(texture),
mColor(color),
mTile(tile)
{
	assert(texture != nullptr);
}


MaterialEffectDiffuseTexture::~MaterialEffectDiffuseTexture()
{
}

ITexture* MaterialEffectDiffuseTexture::GetDiffuseTexture() const
{
	return mTexture;
}
const glm::vec3 MaterialEffectDiffuseTexture::GetColor() const
{
	return mColor;
}

float MaterialEffectDiffuseTexture::GetTile() const
{
	return mTile;
}

MaterialEffectDiffuseTexture* MaterialEffectDiffuseTexture::DoClone() const
{
	return new MaterialEffectDiffuseTexture(*this);
}