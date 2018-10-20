#pragma once
#include "../IMaterialEffect.h"

class TextureCubemap;

class MaterialEffectTextureCubemap : public IMaterialEffect
{
	TextureCubemap* mTexture1;
	TextureCubemap* mTexture2;
	float mBlendFactor;

public:
	MaterialEffectTextureCubemap(TextureCubemap* textureCubemap1, TextureCubemap* textureCubemap2, float blendFactor);
	virtual ~MaterialEffectTextureCubemap();

	TextureCubemap* GetCubemap1() const;
	TextureCubemap* GetCubemap2() const;
	float GetBlendFactor() const;
	void SetBlendFactor(float blendFactor);
	void SetCubemap1(TextureCubemap* texture);
	void SetCubemap2(TextureCubemap* texture);

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectTextureCubemap* DoClone() const override;
};

