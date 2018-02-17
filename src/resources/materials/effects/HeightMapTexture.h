#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class HeightMapTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;

public:
	HeightMapTexture(ITexture* texture, float tile);
	virtual ~HeightMapTexture();

	ITexture* GetHeightMapTexture() const;
	float GetTile() const;

	HeightMapTexture* DoClone() const override;
};

