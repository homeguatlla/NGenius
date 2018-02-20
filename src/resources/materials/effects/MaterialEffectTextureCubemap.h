#pragma once
#include "../IMaterialEffect.h"

class TextureCubemap;

class MaterialEffectTextureCubemap : public IMaterialEffect
{
	TextureCubemap* mTexture;

public:
	MaterialEffectTextureCubemap(TextureCubemap* textureCubemap);
	virtual ~MaterialEffectTextureCubemap();

	TextureCubemap* GetCubemap() const;

	MaterialEffectTextureCubemap* DoClone() const override;
};

