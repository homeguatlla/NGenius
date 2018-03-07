#include "stdafx.h"
#include "MaterialEffectTextureArray.h"
#include "../IMaterial.h"
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

void MaterialEffectTextureArray::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectTextureArray* effect = material->GetEffect<MaterialEffectTextureArray>();
	if (effect != nullptr)
	{
		mTexture = effect->GetTextureArray();
	}
}

MaterialEffectTextureArray* MaterialEffectTextureArray::DoClone() const
{
	return new MaterialEffectTextureArray(*this);
}