#include "stdafx.h"
#include "MaterialEffectTextureCubemap.h"
#include "../IMaterial.h"

#include <assert.h>

MaterialEffectTextureCubemap::MaterialEffectTextureCubemap(TextureCubemap* textureCubemap1, TextureCubemap* textureCubemap2, float blendFactor) :
mTexture1(textureCubemap1),
mTexture2(textureCubemap2),
mBlendFactor(blendFactor)
{
	assert(textureCubemap1 != nullptr);
	if (textureCubemap2 == nullptr)
	{
		blendFactor = 0.0f;
	}
}

MaterialEffectTextureCubemap::~MaterialEffectTextureCubemap()
{
}

TextureCubemap* MaterialEffectTextureCubemap::GetCubemap1() const
{
	return mTexture1;
}

TextureCubemap* MaterialEffectTextureCubemap::GetCubemap2() const
{
	return mTexture2;
}

float MaterialEffectTextureCubemap::GetBlendFactor() const
{
	return mBlendFactor;
}

void MaterialEffectTextureCubemap::SetBlendFactor(float blendFactor)
{
	mBlendFactor = blendFactor;
}

void MaterialEffectTextureCubemap::SetCubemap1(TextureCubemap* texture)
{
	mTexture1 = texture;
}

void MaterialEffectTextureCubemap::SetCubemap2(TextureCubemap* texture)
{
	mTexture2 = texture;
}

void MaterialEffectTextureCubemap::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectTextureCubemap* effect = material->GetEffect<MaterialEffectTextureCubemap>();
	if (effect != nullptr)
	{
		mTexture1 = effect->GetCubemap1();
		mTexture2 = effect->GetCubemap2();
		mBlendFactor = effect->GetBlendFactor();
	}
}

MaterialEffectTextureCubemap* MaterialEffectTextureCubemap::DoClone() const
{
	return new MaterialEffectTextureCubemap(*this);
}