#include "stdafx.h"
#include "MaterialEffectTextureCubemap.h"
#include "../IMaterial.h"

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

void MaterialEffectTextureCubemap::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectTextureCubemap* effect = material->GetEffect<MaterialEffectTextureCubemap>();
	if (effect != nullptr)
	{
		mTexture = effect->GetCubemap();
	}
}

MaterialEffectTextureCubemap* MaterialEffectTextureCubemap::DoClone() const
{
	return new MaterialEffectTextureCubemap(*this);
}