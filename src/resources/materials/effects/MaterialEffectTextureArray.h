#pragma once
#include "../IMaterialEffect.h"

class TextureArray;

class MaterialEffectTextureArray : public IMaterialEffect
{
	TextureArray* mTexture;

public:
	MaterialEffectTextureArray(TextureArray* texture);
	virtual ~MaterialEffectTextureArray();

	TextureArray* GetTextureArray() const;

	MaterialEffectTextureArray* DoClone() const override;
};

