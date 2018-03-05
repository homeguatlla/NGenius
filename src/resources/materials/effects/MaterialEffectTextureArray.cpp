#include "stdafx.h"
#include "MaterialEffectTextureArray.h"
#include <assert.h>

MaterialEffectTextureArray::MaterialEffectTextureArray(TextureArray* textureArray) :
mTexture(textureArray)
{
	assert(textureArray != nullptr);
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