#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class NormalTexture : public IMaterialEffect
{
	ITexture* mTexture;
	int mTile;

public:
	NormalTexture(ITexture* texture, int tile);
	virtual ~NormalTexture();

	ITexture* GetNormalTexture() const;
	int GetTile() const;

	NormalTexture* DoClone() const override;
};

