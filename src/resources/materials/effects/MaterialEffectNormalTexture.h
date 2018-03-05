#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class MaterialEffectNormalTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;

public:
	MaterialEffectNormalTexture(ITexture* texture, float tile);
	virtual ~MaterialEffectNormalTexture();

	ITexture* GetNormalTexture() const;
	float GetTile() const;

	MaterialEffectNormalTexture* DoClone() const override;
};

