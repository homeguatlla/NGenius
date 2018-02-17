#include "stdafx.h"
#include "TextureArrayMaterialEffect.h"

TextureArrayMaterialEffect::TextureArrayMaterialEffect(TextureArray* textureArray) :
mTexture(textureArray)
{
}


TextureArrayMaterialEffect::~TextureArrayMaterialEffect()
{
}

TextureArray* TextureArrayMaterialEffect::GetTextureArray() const
{
	return mTexture;
}

TextureArrayMaterialEffect* TextureArrayMaterialEffect::DoClone() const
{
	return new TextureArrayMaterialEffect(*this);
}