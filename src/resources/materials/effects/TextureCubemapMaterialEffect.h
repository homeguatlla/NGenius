#pragma once
#include "../IMaterialEffect.h"

class TextureCubemap;

class TextureCubemapMaterialEffect : public IMaterialEffect
{
	TextureCubemap* mTexture;

public:
	TextureCubemapMaterialEffect(TextureCubemap* textureCubemap);
	virtual ~TextureCubemapMaterialEffect();

	TextureCubemap* GetCubemap() const;

	TextureCubemapMaterialEffect* DoClone() const override;
};

