#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class DiffuseTexture : public IMaterialEffect
{
	ITexture* mTexture;
	int mTile;

public:
	DiffuseTexture(ITexture* texture, int tile);
	virtual ~DiffuseTexture();

	ITexture* GetDiffuseTexture() const;
	int GetTile() const;

	DiffuseTexture* DoClone() const override;
};

