#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class MaterialEffectDepthTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;

public:
	MaterialEffectDepthTexture(ITexture* texture, float tile);
	virtual ~MaterialEffectDepthTexture();

	ITexture* GetDepthTexture() const;
	float GetTile() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectDepthTexture* DoClone() const override;
};
