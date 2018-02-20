#include "stdafx.h"
#include "MaterialEffectTextureArray.h"

MaterialEffectTextureArray::MaterialEffectTextureArray(TextureArray* textureArray) :
mTexture(textureArray)
{
}


MaterialEffectTextureArray::~MaterialEffectTextureArray()
{
}

TextureArray* MaterialEffectTextureArray::GetTextureArray() const
{
	return mTexture;
}

MaterialEffectTextureArray* MaterialEffectTextureArray::DoClone() const
{
	return new MaterialEffectTextureArray(*this);
}