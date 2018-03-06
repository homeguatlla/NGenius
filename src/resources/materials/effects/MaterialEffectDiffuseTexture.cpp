#include "stdafx.h"
#include "MaterialEffectDiffuseTexture.h"

MaterialEffectDiffuseTexture::MaterialEffectDiffuseTexture()
{

}

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

void MaterialEffectDiffuseTexture::SetDiffuseTexture(ITexture* texture)
{
	mTexture = texture;
}

const glm::vec3 MaterialEffectDiffuseTexture::GetColor() const
{
	return mColor;
}

float MaterialEffectDiffuseTexture::GetTile() const
{
	return mTile;
}

void MaterialEffectDiffuseTexture::SetTile(float tile)
{
	mTile = tile;
}

MaterialEffectDiffuseTexture* MaterialEffectDiffuseTexture::DoClone() const
{
	return new MaterialEffectDiffuseTexture(*this);
}