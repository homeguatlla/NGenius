#include "stdafx.h"
#include "TextureCubemapMaterialEffect.h"

TextureCubemapMaterialEffect::TextureCubemapMaterialEffect(TextureCubemap* textureCubemap) :
mTexture(textureCubemap)
{
}


TextureCubemapMaterialEffect::~TextureCubemapMaterialEffect()
{
}

TextureCubemap* TextureCubemapMaterialEffect::GetCubemap() const
{
	return mTexture;
}

TextureCubemapMaterialEffect* TextureCubemapMaterialEffect::DoClone() const
{
	return new TextureCubemapMaterialEffect(*this);
}