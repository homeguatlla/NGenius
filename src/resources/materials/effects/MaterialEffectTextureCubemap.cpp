#include "stdafx.h"
#include "MaterialEffectTextureCubemap.h"
#include <assert.h>

MaterialEffectTextureCubemap::MaterialEffectTextureCubemap(TextureCubemap* textureCubemap) :
mTexture(textureCubemap)
{
	assert(textureCubemap != nullptr);
}


MaterialEffectTextureCubemap::~MaterialEffectTextureCubemap()
{
}

TextureCubemap* MaterialEffectTextureCubemap::GetCubemap() const
{
	return mTexture;
}

MaterialEffectTextureCubemap* MaterialEffectTextureCubemap::DoClone() const
{
	return new MaterialEffectTextureCubemap(*this);
}