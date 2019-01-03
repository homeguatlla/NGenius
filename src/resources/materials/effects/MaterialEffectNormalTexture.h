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
	void SetNormalTexture(ITexture* texture);
	float GetTile() const;
	void SetTile(float tile);

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectNormalTexture* DoClone() const override;
};

