#include "stdafx.h"
#include "MaterialEffectDiffuseTexture.h"
#include "../IMaterial.h"

MaterialEffectDiffuseTexture::MaterialEffectDiffuseTexture() :
	mTexture(nullptr),
	mColor(glm::vec3(0.0f)),
	mTile(1.0f)
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

void MaterialEffectDiffuseTexture::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		mTexture = effectDiffuse->GetDiffuseTexture();
		mTile = effectDiffuse->GetTile();
		mColor = effectDiffuse->GetColor();
	}
}

MaterialEffectDiffuseTexture* MaterialEffectDiffuseTexture::DoClone() const
{
	return new MaterialEffectDiffuseTexture(*this);
}