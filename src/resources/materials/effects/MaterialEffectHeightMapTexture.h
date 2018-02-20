#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class MaterialEffectHeightMapTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;

public:
	MaterialEffectHeightMapTexture(ITexture* texture, float tile);
	virtual ~MaterialEffectHeightMapTexture();

	ITexture* GetHeightMapTexture() const;
	float GetTile() const;

	MaterialEffectHeightMapTexture* DoClone() const override;
};

