#include "stdafx.h"
#include "MaterialEffectTextureCubemap.h"

MaterialEffectTextureCubemap::MaterialEffectTextureCubemap(TextureCubemap* textureCubemap) :
mTexture(textureCubemap)
{
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