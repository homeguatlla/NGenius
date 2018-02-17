#pragma once
#include "../IMaterialEffect.h"

class TextureArray;

class TextureArrayMaterialEffect : public IMaterialEffect
{
	TextureArray* mTexture;

public:
	TextureArrayMaterialEffect(TextureArray* texture);
	virtual ~TextureArrayMaterialEffect();

	TextureArray* GetTextureArray() const;

	TextureArrayMaterialEffect* DoClone() const override;
};

