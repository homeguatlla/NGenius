#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class NormalTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;

public:
	NormalTexture(ITexture* texture, float tile);
	virtual ~NormalTexture();

	ITexture* GetNormalTexture() const;
	float GetTile() const;

	NormalTexture* DoClone() const override;
};

